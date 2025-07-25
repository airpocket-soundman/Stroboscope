#include <M5Unified.h>
#include <SD.h>

const int FRAME_COUNT = 199;      // 再生する画像の枚数
const int FRAME_DELAY = 0;     // フレーム間の待機時間（ミリ秒）

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setRotation(0);
  M5.Display.setTextSize(2);

  // SDカード初期化（M5Stack Core2は GPIO_NUM_4）
  if (!SD.begin(GPIO_NUM_4)) {
    M5.Display.println("❌ SD init failed");
    while (true);
  }
}

void loop() {
  for (int i = 1; i < FRAME_COUNT; i++) {
    char filename[16];
    sprintf(filename, "/%04d.bmp", FRAME_COUNT-i + 1);  // 読み込むbmpファイル名を生成　例: /0000.bmp ～ 0198.bmp

    File f = SD.open(filename);                         // bmpファイルを開く
    if (!f) {
      M5.Display.printf("❌ File open failed: %s\n", filename);
      delay(500);
      continue;
    }

    size_t len = f.size();                              // ファイルサイズを取得
    uint8_t* buffer = (uint8_t*)malloc(len);            // ファイルサイズ分のバッファを確保
    if (!buffer) {
      M5.Display.println("❌ Memory allocation failed");
      f.close();
      delay(500);
      continue;
    }

    f.read(buffer, len);                                // ファイル全体をバッファに読み込む
    f.close();

    // 拡大なしでそのまま左上に表示
    M5.Display.drawBmp(buffer, len, 0, 40);             // バッファを開放する.
    delay(FRAME_DELAY);
  }
}

