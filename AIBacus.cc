#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Bacus


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  const vector<Dir> dirs = {Up,Down,Left,Right};

  bool pos_finding_the_queen(Pos p){
    if (cell(p).type != Water) {
      if(cell(p).id == -1) return true;
      else if (ant(cell(p).id).player == me()){
        if (cell(p).id == queens(me())[0]) return true;
      }
    }
    return false;
  }

  bool pos_ok_general(Pos p){
    return cell(p).type != Water and cell(p).id == -1;
  }
  
  bool pos_ok_soldiers(Pos p, const set<Pos>& my_ants){
    if (cell(p).id != -1 and ant(cell(p).id).player != me() and ant(cell(p).id).type == Worker) return true;
    else if (my_ants.find(p) != my_ants.end()) return false;
    else if (cell(p).type != Water and cell(p).id == -1) return true;
    return false;
  }
  bool pos_ok_queen_of_hearts(Pos p, const set<Pos>& my_ants){
    if (cell(p).id != -1 and ant(cell(p).id).player != me() and ant(cell(p).id).type != Queen) return true;
    else if (my_ants.find(p) != my_ants.end()) return false;
    else if (cell(p).type != Water and cell(p).id == -1) return true;
    return false;
  }

  typedef vector<vector<pair<bool, Pos>>> Matriu;

  Pos bfs_bonus(Matriu& path, int idw, const set<Pos>& my_ants){
    queue<Pos> Q;
    Pos ini = ant(idw).pos;
    Q.push(ini);
    while (not Q.empty()){
      Pos a = Q.front();
      Q.pop();
      for(int i = 0; i < 4; ++i){
        Pos next = a + dirs[i];
        if(pos_ok(next) and pos_ok_general(next) and not path[next.i][next.j].first){
          Q.push(next);
          path[next.i][next.j].first = true;
          path[next.i][next.j].second = a;
          if (cell(next).bonus == Bread) return next;
          else if(cell(next).bonus == Seed) return next;
          else if (cell(next).bonus == Leaf) return next;
        }
      }
    }
  return {-1,-1};

  }

Pos bfs_queen_bonus(Matriu& path, int idw, const set<Pos>& my_ants){
    queue<Pos> Q;
    Pos ini = ant(idw).pos;
    Q.push(ini);
    while (not Q.empty()){
      Pos a = Q.front();
      Q.pop();
      for(int i = 0; i < 4; ++i){
        Pos next = a + dirs[i];
        if(pos_ok(next) and pos_ok_queen_of_hearts(next, my_ants) and not path[next.i][next.j].first){
          Q.push(next);
          path[next.i][next.j].first = true;
          path[next.i][next.j].second = a;
          if (cell(next).bonus != None) return next;
        }
      }
    }
  return {-1,-1};

  }

  Pos bfs_kill_soldiers(Matriu& path, int ids, const set<Pos>& my_ants){
    queue<Pos> Q;
    path = Matriu(board_rows(), vector<pair<bool, Pos>>(board_cols(), {false, {-1,-1}}));
    Pos ini = ant(ids).pos;
    Q.push(ini);
    path[ini.i][ini.j].first = true;
    path[ini.i][ini.j].second = ini;
    while (not Q.empty()){
      Pos a = Q.front();
      Q.pop();
      for(int i = 0; i < 4; ++i){
        Pos next = a + dirs[i];
        if(pos_ok(next) and pos_ok_soldiers(next, my_ants) and not path[next.i][next.j].first){
          Q.push(next);
          path[next.i][next.j].first = true;
          path[next.i][next.j].second = a;
          if (cell(next).id != -1 and ant(cell(next).id).player != me() and  ant(cell(next).id).type == Worker) return next;
        }
      }
    }
  return {-1,-1};

  }
Pos bfs_kill_queen(Matriu& path, int ids, const set<Pos>& my_ants){
    queue<Pos> Q;
    path = Matriu(board_rows(), vector<pair<bool, Pos>>(board_cols(), {false, {-1,-1}}));
    Pos ini = ant(ids).pos;
    Q.push(ini);
    path[ini.i][ini.j].first = true;
    path[ini.i][ini.j].second = ini;
    while (not Q.empty()){
      Pos a = Q.front();
      Q.pop();
      for(int i = 0; i < 4; ++i){
        Pos next = a + dirs[i];
        if(pos_ok(next) and pos_ok_queen_of_hearts(next, my_ants) and not path[next.i][next.j].first){
          Q.push(next);
          path[next.i][next.j].first = true;
          path[next.i][next.j].second = a;
          if (cell(next).id != -1 and ant(cell(next).id).player != me() and  ant(cell(next).id).type != Queen) return next;
        }
      }
    }
  return {-1,-1};

  }


  Pos bfs_to_queen(Matriu& path, int idw, const set<Pos>& my_ants){
    queue<Pos> Q;
    path = Matriu(board_rows(), vector<pair<bool, Pos>>(board_cols(), {false, {-1,-1}}));
    Pos ini = ant(idw).pos;
    Q.push(ini);
    path[ini.i][ini.j].first = true;
    path[ini.i][ini.j].second = ini;
    while (not Q.empty()){
      Pos a = Q.front();
      Q.pop();
      for(int i = 0; i < 4; ++i){
        Pos next = a + dirs[i];
        if(pos_ok(next) and pos_finding_the_queen(next) and not path[next.i][next.j].first){
          Q.push(next);
          path[next.i][next.j].first = true;
          path[next.i][next.j].second = a;
          if (cell(next).id == queens(me())[0]) return next;
        }
      }
    }
  return {-1,-1};

  }
bool am_i_around_the_queen(Pos ini, int queen){
  for(int i = 0; i < 4; ++i){
    Pos next = ini + dirs[i];
    if (pos_ok(next) and (next == ant(queen).pos)) return true;
  }
  return false;
}

//around the queen is visited, we don't care about the bonus
void the_court(Matriu& path, int queen){
    for (int i = 0; i < 4; ++i){
      Pos next = ant(queen).pos + dirs[i];
      if(pos_ok(next)) path[next.i][next.j].first = true;
    }
}
void easy_move(Pos p, int idw){
  Dir d = dirs[random(0,3)];
  int i = p.i - ant(idw).pos.i;
  int j = p.j - ant(idw).pos.j;
  if (i == 1) d = Down;
  else if (i == -1) d = Up;
  else if (j == 1) d = Right;
  else if (j == -1) d = Left;
  move(idw, d);
       
}

bool reina_tranqui(){
  for (int i = 0; i < 4; ++i){
    Pos next = ant(queens(me())[0]).pos + dirs[i];
    if (pos_ok(next) and cell(next).id != -1 and ant(cell(next).id).player == me() and ant(cell(next).id).bonus != None) return true;
  }
  return false;
}


void move2(int id, Matriu& path, Pos& p, set<Pos>my_ants){
    Dir d = dirs[random(0,3)];
    Pos ini = ant(id).pos;
    bool found = false;
    while (path[p.i][p.j].second != ini) p = path[p.i][p.j].second;
    if(my_ants.find(p) == my_ants.end()){
      int i = p.i - ini.i;
      int j = p.j - ini.j;
      if (i == 1) d = Down;
      else if (i == -1) d = Up;
      else if (j == 1) d = Right;
      else if (j == -1) d = Left;
      move(id, d);
    }
}
/*bool soldiers_be_careful(Pos p){
  for(int i = 0; i < 4; ++i){
    Pos next = p + dirs[i];
    if(pos_ok(next) and cell(next).id != -1){
      if(ant(cell(p).id).player != me() and ant(cell(p).id).type != Worker) return false;
    }
  }
  return true;

}*/

/*bool workers_be_careful(Pos p){
  for(int i = 0; i < 4; ++i){
    Pos next = p + dirs[i];
    if(pos_ok(next) and cell(next).id != -1) return false;
    }
  return true;
}*/

char enough_food(bool& birth){
  char c = 'x';
  int queen = queens(me())[0];
  if (round() < 50) c = 's';
  else if (round () >= 50 and round () < 100) c = 'w';
  else if (round () >= 100 and round() < 150) c = 's';
  else if (round() >= 150) c = 'w';
  if (c == 's' and ant(queen).reserve[0] >= soldier_carbo() and ant(queen).reserve[1] >= soldier_lipid() and ant(queen).reserve[2] >= soldier_prote()) birth = true;
  else if (c == 'w' and ant(queen).reserve[0] >= worker_carbo() and ant(queen).reserve[1] >= worker_lipid() and ant(queen).reserve[2] >= worker_prote()) birth = true;
  return c;
}
void easy_lay(Pos p, char c){
  Dir d = dirs[random(0,3)];
  int i = p.i - ant(queens(me())[0]).pos.i;
  int j = p.j - ant(queens(me())[0]).pos.j;
  if (i == 1) d = Down;
  else if (i == -1) d = Up;
  else if (j == 1) d = Right;
  else if (j == -1) d = Left;
  if (c == 'w') lay (queens(me())[0], d, Worker); 
  else if (c == 's') lay (queens(me())[0], d, Soldier); 
}

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    int queen = queens(me())[0]; 
    vector<int>workants = workers(me());
    vector<int>soldants = soldiers(me());
    set<Pos>my_ants;
    my_ants.insert(ant(queen).pos);
    for (unsigned int i = 0; i < workants.size(); ++i) my_ants.insert(ant(workants[i]).pos);
    for (unsigned int i = 0; i < soldants.size(); ++i) my_ants.insert(ant(soldants[i]).pos);
    
    //LA PROLE
    for (int idw : workants){
      Matriu path;
      Pos p = {-1, -1};
      bool kween = false;
      if (ant(idw).bonus != None){
        for (int i = 0; i < 4; ++i){
          Pos next = ant(idw).pos + dirs[i];
          if(pos_ok(next) and pos_finding_the_queen(next) and cell(ant(idw).pos).bonus == None){
            if(cell(next).id == queen) {
              kween = true;
              break;
            }
          }
        }
        if (kween) leave(idw);
        else {
          p = bfs_to_queen(path, idw, my_ants);
          if (p.i != -1) {
            move2(idw, path, p, my_ants);
            my_ants.insert(p);
          }
        }
      }
      else if (cell((ant(idw).pos)).bonus != None){
        if(not am_i_around_the_queen(ant(idw).pos, queen)) take(idw);
        else{
          path = Matriu(board_rows(), vector<pair<bool, Pos>>(board_cols(), {false, {-1,-1}}));
          path[ant(idw).pos.i][ant(idw).pos.j].first = true;  
          path[ant(idw).pos.i][ant(idw).pos.j].second = ant(idw).pos; 
          the_court(path, queen);         
          p = bfs_bonus(path, idw, my_ants);
          if (p.i != -1){
            move2(idw, path, p, my_ants);
            my_ants.insert(p);
          }  
        }
      }

      else{
        path = Matriu(board_rows(), vector<pair<bool, Pos>>(board_cols(), {false, {-1,-1}}));
        the_court(path, queen);         
        for (int i = 0; i < 4; ++i){
          Pos next = ant(idw).pos + dirs[i];
          if(pos_ok(next) and pos_ok_general(next) and (my_ants.find(next) == my_ants.end())){
            if(cell(next).bonus != None and not am_i_around_the_queen(ant(idw).pos, queen) and not path[next.i][next.j].first) {
              p = next;
              my_ants.insert(p);
              break;
            }
          }
        }
        if (p.i != -1) easy_move(p, idw);
        else{
          path[ant(idw).pos.i][ant(idw).pos.j].first = true;
          path[ant(idw).pos.i][ant(idw).pos.j].second = ant(idw).pos;
          p = bfs_bonus(path, idw, my_ants);
          if (p.i != -1){
            move2(idw, path, p, my_ants);
            my_ants.insert(p);
          }
        }
      }
    }
    //YAS QUEEN
    if(round() % queen_period() == 0){
      Pos pqueen = {-1,-1};
      Matriu path;
      bool birth = false;
      char c =  enough_food(birth);

      if (birth){
        for (int i = 0; i < 4; ++i){
          Pos next = ant(queen).pos + dirs[i];
          if (pos_ok(next) and pos_ok_general(next) and my_ants.find(next) == my_ants.end()){
            pqueen = next;
            my_ants.insert(pqueen);
            break;
          }
        }
        if (pqueen.i != -1) easy_lay(pqueen, c);

      }
      else{    
        for (int i = 0; i < 4; ++i){
          Pos next = ant(queen).pos + dirs[i];
          if(pos_ok(next) and pos_ok_queen_of_hearts(next, my_ants)){
            if(cell(next).bonus != None) {
              pqueen = next;
              my_ants.insert(pqueen);
              break;
            }
          }
        }
        if (pqueen.i != -1) easy_move(pqueen, queen);
        else if(not reina_tranqui()) {
          if (workants.size() < 2){
            path = Matriu(board_rows(), vector<pair<bool, Pos>>(board_cols(), {false, {-1,-1}}));
            Pos pqueen = bfs_queen_bonus(path, queen, my_ants);
            if (pqueen.i != -1) {
              move2(queen, path, pqueen, my_ants);
              my_ants.insert(pqueen);
            }
            else {
              path = Matriu(board_rows(), vector<pair<bool, Pos>>(board_cols(), {false, {-1,-1}}));
              Pos pqueen = bfs_kill_queen(path, queen, my_ants);
              if (pqueen.i != -1) {
                move2(queen, path, pqueen, my_ants);
                my_ants.insert(pqueen);
              }
            }          
          }
          else{
            Pos pqueen = bfs_kill_queen(path, queen, my_ants);
            if (pqueen.i != -1) {
              move2(queen, path, pqueen, my_ants);
              my_ants.insert(pqueen);
            }
          }
        }
      }
    }

//FFCC DE SEGURIDAD DEL ESTADO
    for(int ids : soldants){
      Pos pos = {-1,-1};
      Matriu paths;
      for (int i = 0; i < 4; ++i){
        Pos next = ant(ids).pos + dirs[i];
        if (pos_ok(next) and cell(next).type != Water and pos_ok_soldiers(next, my_ants) and (my_ants.find(next) == my_ants.end())){
          if (cell(next).id != -1) {
            pos = next;
            my_ants.insert(pos);
            break;
          } 
        }
      }

      if(pos.i != -1) easy_move(pos, ids);
      else {
        pos = bfs_kill_soldiers(paths, ids, my_ants);
        if (pos.i != -1){
          move2(ids, paths, pos, my_ants);
          my_ants.insert(pos);
        }
      }
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
