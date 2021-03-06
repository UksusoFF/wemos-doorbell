# WeMos Doorbell
Простой дверной звонок основанный на Arduino-совместимой плате WeMos D1 R2. 

Умеет играть мелодию через пищалку и присылать уведомления на телефон.

Можно через оптрон 4N25 подключить домофон Метаком ТКП-12D (или любой другой который моргает диодом при звонке).

Мелодии используются в формате [MML](https://en.wikipedia.org/wiki/Music_Macro_Language) с сайта [ArcheAge MML Library](https://archeagemmllibrary.com/).

Для уведомлений используется сервис [Pushetta](http://www.pushetta.com) или [Slack](https://slack.com).

Схема подключения:
![image](https://user-images.githubusercontent.com/1931442/37878855-428fa142-3080-11e8-9af3-dee667024dea.png)

Зависимости:
* [ESP8266 Arduino](https://github.com/esp8266/Arduino) v2.7.1 <sup>[ru](https://geekelectronics.org/arduino/wemos-d1-r2-na-esp8266.html)</sup>
* [Bounce2](https://github.com/thomasfredericks/Bounce2) v2.53.0
* [MmlMusic](https://github.com/maxint-rd/MmlMusic) v0.2.0
* [MmlMusicPWM](https://github.com/maxint-rd/MmlMusicPWM) v0.2.0
* [Arduino Json](https://arduinojson.org/doc/) v5.13.5
* [Fritzing Part: WeMos D1 R2](https://github.com/mikeipin/Fritzing-Part-WeMos-D1-R2)

Статьи по теме:
* Устройство и ремонт трубки домофона [Часть 1](http://sesaga.ru/ustrojstvo-i-remont-trubki-domofona.html), [Часть 2](http://sesaga.ru/ustrojstvo-i-remont-trubki-domofona-chast-2.html)
* [Как я домофон Vizit к mqtt подключал](https://geektimes.ru/post/284302/)
* [Чем заняться в выходные: дистанционное управление домофоном](https://geektimes.ru/post/284110/)
* [Домофонный мультиключ и всё про имитацию «таблеток»](https://geektimes.ru/post/258674/)
* [Прокачиваем домофон протоколом MQTT для управления с телефона](https://geektimes.ru/post/292735/)
* [Музыкальный дверной звонок в стиле Star Wars на Arduino](https://geektimes.ru/post/258740/)
* [Управление координатным домофоном Vizit (Cyfral)](https://blog.instalator.ru/archives/971)
