# WeMos Doorbell
Простой дверной звонок основанный на Arduino-совместимой плате WeMos D1 R2. 

Умеет играть мелодию через пищалку и присылать уведомления на телефон.

Можно подключить домофон Метаком ТКП-12D (или любой другой который моргает диодом при звонке).

Мелодии используются в формате [MML](https://en.wikipedia.org/wiki/Music_Macro_Language) с сайта [ArcheAge MML Library](https://archeagemmllibrary.com/).

Для уведомлений используется сервис [Pushetta](http://www.pushetta.com).

Зависимости:
* [ESP8266 Arduino](https://geekelectronics.org/arduino/wemos-d1-r2-na-esp8266.html)
* [Bounce2](https://github.com/thomasfredericks/Bounce2)
* [MmlMusicPWM](https://github.com/maxint-rd/MmlMusicPWM)
