# scat

Simple constructor for acceptance tests

[![scat](https://github.com/ivorob/scat/actions/workflows/cmake.yml/badge.svg)](https://github.com/ivorob/scat/actions)

Новая концепция тестов:
* Тестовый файл может содержать один сценарий;
* Сценарий имеет следующий вид:
  ```
  Scenario: Описывает реализуемый сценарий
  Given: Начальные условия
  When: Действие, запускающее сценарий
  Then: Результат
  ```
* Ключевое слово Given - опциональный элемент сценария;
* В сценарии возможно использовать ключевое слово And для комбинации условий;
* В разделе Given можно ссылаться на другие тесты, создавая зависимости, для это следует использовать следующую маску #ref("<имя файла сценария>"). 

## Usage
```
usage:
	scat testfile
```

Example of executing scenario:
```
[   Scenario ]	Install package
[------------]	----------------------------------
[      Given ]	Clean machine
[       When ]	Execute installation for package
[       Then ]	Packages files are present
[ RUN        ]
[         OK ]
[------------]
```
