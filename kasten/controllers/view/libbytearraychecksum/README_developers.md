# How to add a new checksum algorithm

### 1. Creating source files

Copy the template files `template_bytearraychecksumalgorithm.*` from  
    [okteta/kasten/controllers/view/libbytearraychecksum/algorithm/template/](algorithm/template/)  
to  
    [okteta/kasten/controllers/view/libbytearraychecksum/algorithm/](algorithm/)  
using new file names matching the class name.
E.g. for `MyByteArrayChecksumAlgorithm` use `mybytearraychecksumalgorithm.*`.


### 2. Adapting the code from the template

In the templates the ranges where you need to do the changes are marked with
```cpp
//// ADAPT(start)
//// ADAPT(end)
```
Follow the advises given at the beginning of each range, and use
`NoByteArrayChecksumParameterSet` as the parameter set.


### 3. Adding the checksum algorithm to the library

Grep these files for the string `NEWCHECKSUM` to find where you need to make
changes to add your checksum algorithm, and follow the advises given there:
* [okteta/kasten/controllers/CMakeLists.txt](../../CMakeLists.txt)
* [okteta/kasten/controllers/view/libbytearraychecksum/bytearraychecksumalgorithmfactory.cpp](bytearraychecksumalgorithmfactory.cpp)


### 4. Compile, install and run

Now you are ready to enter the cycle of compile, test, and fix. Do so :)


### 5. Share your results

If you are pleased with your results, contact the maintainer of Okteta, as found
in the "About Okteta" dialog, for inclusion in the official sources.


TODO: A test system to make sure your checksum does what it is supposed to.



# How to add a new checksum algorithm with new parameterset


### 0. Create a checksum algorithm with fixed parameters

As a start do the steps 1.-4, as described above and just set the parameters to fixed values,
until the checksum basically works.


### 1. Creating source files

Copy the template files `template_bytearraychecksumparameterset{,edit}.*` from  
    [okteta/kasten/controllers/view/libbytearraychecksum/algorithm/template/](algorithm/template/)  
to  
    [okteta/kasten/controllers/view/libbytearraychecksum/algorithm/](algorithm/)  
using new file names matching the class name.
E.g. for `MyByteArrayChecksumParameterSet` use `mybytearraychecksumparameterset{,edit}.*`.


### 2. Adapting the code from the template

In the templates the ranges where you need to do the changes are marked with
```cpp
//// ADAPT(start)
//// ADAPT(end)
```
Follow the advises given at the beginning of each range.


### 3. Adding the parameterset to the library

Grep these files for the string `NEWCHECKSUMPARAMETERSET` to find where you need to make
changes to add your checksum algorithm, and follow the advises given there:
* [okteta/kasten/controllers/CMakeLists.txt](../../CMakeLists.txt)
* [okteta/kasten/controllers/view/libbytearraychecksum/bytearraychecksumparameterseteditfactory.cpp](bytearraychecksumparameterseteditfactory.cpp)

Also change your checksum to now make use of the parameter set, by switching from
`NoByteArrayFilterParameterSet` to the one you have created and adapt the `checksum()` method
of your checksum class to take the actual parameter from the parameter set instead of using
the fixed values as done for the start.


### 4. Compile, install and run

Now you are ready to enter the cycle of compile, test, and fix. Do so :)


### 5. Share your results

If you are pleased with your results, contact the maintainer of Okteta, as found
in the "About Okteta" dialog, for inclusion in the official sources.
