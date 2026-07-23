/**
 * Raylib实现的生命游戏：
 * 		使用鼠标设置初始状态，回车：开始演化（S：单步演化）， R:复位
 * 生命游戏，又称康威生命游戏，是英国数学家约翰·何顿·康威在1970年发明的细胞自动机
 */
#include <raylib.h>
#include <string>
int fps=10;
class LifeGame {
 public:
  LifeGame(int fps = 10, std::string title = "lifegame") {
    InitWindow(width, height, title.c_str());
    SetTargetFPS(fps);
  }
  ~LifeGame() { CloseWindow(); }
  void run() {
    while (!WindowShouldClose()) {
      processKeyEvt();    // 处理键盘
      processMouseEvt();  // 处理鼠标
	  
      BeginDrawing();

      drawFrame();  // 绘制当前帧
	  int FPS_temp=GetFPS();
	  DrawText(TextFormat("FPS:%d",FPS_temp),20,20,30,WHITE);
	
	  SetWindowTitle(TextFormat("生命游戏  FPS:%d",FPS_temp));
	
      EndDrawing();

      updateCellStates();  // 更新细胞状态
	  if(IsWindowFocused()){
		SetTargetFPS(fps);
	  }else{
		SetTargetFPS(1);
	  }
    }
  }

 private:
  //
  //  @brief 处理键盘事件
  //
  void processKeyEvt() {
	if (IsKeyPressed(KEY_ENTER)) {  // 回车-运行
		running = !running;
	}
    if (IsKeyPressed(KEY_R)) {  // R - 复位
	    running = false;
	    clearStates();
    }
    if (IsKeyPressed(KEY_S)) {  // S - 单步执行
	    updateCellStates(true);
    }
	if(IsKeyPressed(KEY_UP)){
		fps+=5;
	}else if(IsKeyPressed(KEY_DOWN)){
		fps-=5;
	}
	if(fps>240){
		fps-=5;
	}else if(fps<5){
		fps+=5;
	}
	
  }

  //
  //  @brief 处理鼠标事件：单击改变颜色
  //
  void processMouseEvt() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (!running) {
        Vector2 vec = GetMousePosition();
        int x = vec.x / cell;
        int y = vec.y / cell;
        state[state_index][x + 1][y + 1] = 1;
      }
    }else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		if (!running) {
			Vector2 vec = GetMousePosition();
			int x = vec.x / cell;
			int y = vec.y / cell;
			state[state_index][x + 1][y + 1] = 0;
		}
	}
  }

  //
  //  @brief 绘制当前帧
  //
  void drawFrame() {
    for (int i = 0; i < 60; i++) {
      for (int j = 0; j < 40; j++) {
        Color c = state[state_index][i + 1][j + 1] ? ORANGE : DARKGRAY;
        DrawRectangle(cell * i, cell * j, cell, cell, c);
        DrawRectangleLines(cell * i, cell * j, cell, cell, GREEN);
      }
    }
  }

  //
  //  @brief 更新细胞存活状态
  //
  void updateCellStates(bool force = false) {
    if (!force) {
      if (!running) return;  // 没有运行，直接返回
    }
    int last = state_index;
    state_index = last == 1 ? 0 : 1;

    for (int i = 1; i <= cell_col; i++) {
      for (int j = 1; j <= cell_row; j++) {
        int sum = state[last][i - 1][j - 1] + state[last][i - 1][j] + state[last][i - 1][j + 1] +
                  state[last][i][j - 1] + state[last][i][j + 1] + state[last][i + 1][j - 1] + state[last][i + 1][j] +
                  state[last][i + 1][j + 1];
        if (state[last][i][j]) {
          state[state_index][i][j] = (sum < 2 || sum > 3) ? 0 : 1;
        } else {
          state[state_index][i][j] = (sum == 3) ? 1 : 0;
        }
      }
    }
  }
  //
  //  @brief 清除状态
  //
  void clearStates() {
    state_index = 0;
    for (int i = 1; i <= cell_col; i++) {
      for (int j = 1; j <= cell_row; j++) {
        state[state_index][i][j] = 0;
      }
    }
  }

 public:
  static constexpr float window_size=300;
  static constexpr float width = 4*window_size;
  static constexpr float height = 3*window_size;
  static constexpr float cell = 25;
  static constexpr int cell_col = width / cell;
  static constexpr int cell_row = height / cell;

  char state[2][cell_col + 2][cell_row + 2] = {0};
  int state_index = 0;

  bool running = false;
};

int main(void) {
  LifeGame game;
  game.run();

  return 0;
}
