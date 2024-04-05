# TO DO list

## Basic

- [x] codemapper command line tool parses source code: 
```
$ ./codemapper -t /home/marcelo/dev/personal/github/codemapper/ -l cpp -o png > codemapper.png
```
- [x] General language class 
- [x] C/C++ particular child class
- [x] Python particular child class
- [ ] Python particular child class (remake with "from XX import" exception)
- [ ] Java particular child class
- [ ] :
- [ ] ... (etc) many other languages
- [x] Documentation
- [ ] documentation for users
- [x] documentation for devs
- [ ] Finish documentation
- [ ] Finish all classes with all functionalities

## GUI functionalities

- [x] Show codemapper output
- [x] Node selection
- [x] Edge selection
- [x] Node features edition
- [x] Edge features edition
- [x] Delete nodes and child edges
- [x] cm_graph class delete nodes function
- [ ] adding levels to the graph
- [x] adding features to nodes
- [x] adding features to nodes
- [x] GUI ways of output:
- [x] (1) output as dot
- [x] (2) output as png
- [x] (3) output as svg
- [x] GUI ways of input:
- [x] (4) import a project file saved by (1) now empty in the GUI button
- [x] Automatize color of nodes
- [ ] Automatize color of edges
- [ ] Review the node selection logic and think about the edge way
- [ ] Documentation

## Win32 version

- [x] Prepare CMakeLists.txt adaptation
- [x] Tests and improvements to reach the same behaviour
- [ ] Documentation

## Package releases

- [x] Start DEB package creation (cpack <- cmake)
- [ ] Complete first deb relase and publish
- [ ] Prepare to MSI package creation
- [ ] Compelte first MSI release and publish

# Knowledge of weakness

- [ ] NODE selection and EDGE selection, ways and diferences, why ```node``` selection goes to SVG but ```edge``` selection needs only the ```cm_graph*``` project?
- [ ] About summarize all bin deps to create a DEB or MSI package, any experience before.
- [ ] Other languages where ```codemapper```tools can be adapted only adding the ```cm_LANGUAGElan class```? Java? Rust? PHP? Javascript? If somebody can write it, I'll be really very greatful.
