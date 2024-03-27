# TO DO list

## Basic

- [x] codemapper command line tool parses source code: 
```
$ ./codemapper -t /home/marcelo/dev/personal/github/codemapper/ -l cpp -o png > codemapper.png
```
- [x] General language class 
- [x] C/C++ particular child class
- [x] Python particular child class
- [ ] Java particular child class
- [ ] ... (etc) many other languages
- [x] Documentation
- [ ] Finish documentation
- [ ] Finish all classes with all functionalities

## GUI functionalities

- [x] Show codemapper output
- [x] Node selection
- [x] Edge selection
- [ ] Node features edition
- [x] Edge features edition
- [x] Delete nodes and child edges
- [x] cm_graph class delete nodes function
- [ ] adding levels to the graph
- [ ] adding features to nodes
- [ ] GUI ways of output:
- [x] (1) output as dot
- [x] (2) output as png
- [x] (3) output as svg
- [x] (4) import a project file saved by (1) now empty in the GUI button
- [ ] Documentation

## Win32 version

- [ ] Prepare CMakeLists.txt
- [ ] Tests and improvements to same behaviour
- [ ] Documentation

## Packages 

- [x] Start DEB package creation (cpack <- cmake)
- [ ] Complete first deb relase and publish
- [ ] Prepare to MSI package creation
- [ ] Compelte first MSI release and publish
