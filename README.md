# PS3RemoteController Library

PS3リモコン等を、簡単にArduinoで使用するためのライブラリである、  
「[USB Host Library Rev.2.0](https://github.com/felis/USB_Host_Shield_2.0)」
を、PS3リモコンのみを、簡略化して利用するためのライブラリ。  

# 準備

1. [USB Host Library Rev.2.0](https://github.com/felis/USB_Host_Shield_2.0)
をArduinoのライブラリフォルダにインストールする。  
1. このライブラリを、ライブラリフォルダにインストールする。  
1. ライブラリをインクルードする前に、有線接続(USB)を使う場合は、
	```cpp
	#define PS3_USE_USB
	```
	無線機能(Bluetooth)を使う場合は、
	```cpp
	#define PS3_USE_BT
	```
	を定義する。両方利用したい場合は、両方定義する。
1. `PS3RemoteController.h`をインクルード。  
1. Controllerクラスのインスタンス化  
	リモコンのほとんどの機能は、Controllerクラスによって利用できる。  
	```cpp
	PS3RemoteController::Controller controller;
	```    
1. 初期化  
	setup関数内で、  
	```cpp
	controller.init();
	```  
	を実行する。  
	ここで、必ずエラーチェックを行う必要がある。よって、このように書くのが望ましい。  
	```cpp
	if ( controller.init() == -1 ) // 何らかの処理;
	```
1. メインルーチン  
	loop関数内で、必ず１度は、  
	```cpp
	controller.task();
	```  
	を実行する必要がある。

# リモコンのボタンやジョイスティック、センサの値を取得する  

リモコンのボタンやジョイスティックの値を取得する。
```cpp
controller.update();
```  
リモコンの加速度センサや、ジャイロスコープの値も含めて取得したいときは、  
```cpp
controller.update( 1 );
```  
  
ボタンや、スティックの値は、update関数実行後、  
```cpp
controller.buttonPress[ UP ]
controller.buttonClick[ DOWN ]  
controller.stick.L.x
```  
などの、メンバ変数に格納される。  
  
ボタン状態を格納しているのメンバ変数の配列は、連番で格納されているので、このような書き方ができる。  
```cpp
for ( short i = 0; i < PS3RemoteController::BUTTON_NUM; i++ ) 
```

加速度センサやジャイロスコープの値は、  
```cpp
controller.acc.x  
controller.acc.y  
controller.acc.z  
controller.gyro.z
```  
に格納される。  
軸の向きは、加速度センサは、リモコンの手前側が+Y, 左が+X、上が+Z、  
ジャイロは、CCW方向が+。

# LEDを操作する

```cpp
controller.setLed( 1, 0, 1, 0 );
```  
引数は、左から、LED1~4に対応している。
`setLedToggle`関数を使えば、任意のLEDを、いまのLEDの状態から、反転させることができる。

# バイブレーション機能を利用する

```cpp
controller.setRumble( 1 );
```  
引数の値を1にすればON、0にすればOFFになる。
任意時間バイブをONにし、その後OFFにしたい場合は、
`setRumbleMs`関数を利用できる(`delay`関数を用いているので、その時間処理が停止するから注意)。

# キャリブレーション

```cpp
controller.calibrate();
```
1. LED1~4が点滅し、振動する。
1. 点滅と振動が終了し、LED1が点灯したら、キャリブレーション開始。
1. スティックを中央に戻して、SELECT、STARTボタンを同時に押す。
1. LED2が点灯し、振動する。
1. スティックの最大値、最小値がわかるように、ぐるぐる回す。
1. スティックを中央に戻して、SELECT、STARTボタンを同時に押す。
1. LED1~4が点滅し、振動する。
1. 点滅と振動が終了し、LED1が点灯したら、キャリブレーション終了。

注意：キャリブレーションしたデータは、マイコンをリセットすると無くなる。