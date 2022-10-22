# scat

Simple constructor for acceptance tests

[![scat](https://github.com/ivorob/scat/actions/workflows/cmake.yml/badge.svg)](https://github.com/ivorob/scat/actions)

New conception of tests:
* Test file can contain only one scenario;
* Test scenario looks like as the following:
  ```
  Scenario: Scenario description
  Given: Initial condition
  When: Action, trigger
  Then: The result
  ```
* The keyword _Given_ is the optional item of scenario;
* There is ability to use keyword _And_ inside of scenario to make combination of conditions;
* There is ability to make references to another test scenarios using the following command: `#ref("<name of scenario">)`.

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
Example of executing scenario with command in it:
```
[   Scenario ]	Install package
[------------]	----------------------------------
[      Given ]	There is clean machine
[       When ]	We #execute("install test.pkg -target /")
[       Then ]	Packages files are present)
[ RUN        ]
[         OK ]
[------------]
```

## TODO

* Parse command;
* Execute scenario with command;
* Implement support of keyword _And_;
* Run plugin on executing scenario with command;
* Plugin for running OS command;
* Add test case with command parsing in Scenario for TestFileParser (ignore command in Scenario);
* Keep line and symbol for parsing issues.
