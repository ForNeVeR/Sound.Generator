Звуковой генератор
------------------

Для генерации звука используется отдельное приложение, написанное на C++. Для
его сборки следует выполнить следующие инструкции:

```console
$ msbuild /p:Configuration=Release /p:Platform="x64"
```

API приложения описан в отдельном документе [docs/Sound.Generator.md][].

На целевую машину следует доставлять файлы из каталога
`Sound.Generator/x64/Release`.

### Требования для установки

Для функционирования системы требуется следующее вспомогательное ПО:
- операционная система Windows 7 или выше;
- включенная подсистема речи (экранный диктор) в Windows.

Рекомендуется устанавливать на сервере речевой движок [RHVoice][rhvoice].
Требуется [RHVoice 0.5][download-rhvoice], [русский языковой
пакет][download-rhvoice-russian] и любой из [русских
голосов][download-rhvoice-voices].

[docs/Sound.Generator.md]: docs/Sound.Generator.md

[download-rhvoice]: https://github.com/Olga-Yakovleva/RHVoice/wiki/Downloads-%28Russian%29#%D0%9E%D1%81%D0%BD%D0%BE%D0%B2%D0%BD%D0%BE%D0%B9-%D0%BF%D0%B0%D0%BA%D0%B5%D1%82
[download-rhvoice-russian]: https://github.com/Olga-Yakovleva/RHVoice/wiki/Downloads-%28Russian%29#%D0%AF%D0%B7%D1%8B%D0%BA%D0%BE%D0%B2%D0%BE%D0%B9-%D0%BF%D0%B0%D0%BA%D0%B5%D1%82
[download-rhvoice-voices]: https://github.com/Olga-Yakovleva/RHVoice/wiki/Downloads-%28Russian%29#%D0%93%D0%BE%D0%BB%D0%BE%D1%81%D0%B0
[rhvoice]: http://tiflo.info/rhvoice/
