# How to add a new binary filter

### 1. Creating source files

Copy the template files `template_bytearrayfilter.*` from  
    [okteta/kasten/controllers/view/libbytearrayfilter/filter/template/](filter/template/)  
to  
    [okteta/kasten/controllers/view/libbytearrayfilter/filter/](filter/)  
using new file names matching the class name.
E.g. for `MyByteArrayFilter` use `mybytearrayfilter.*`.


### 2. Adapting the code from the template

In the templates the ranges where you need to do the changes are marked with
```cpp
//// ADAPT(start)
//// ADAPT(end)
```
Follow the advises given at the beginning of each range, and use
`NoByteArrayFilterParameterSet` as the parameter set.


### 3. Adding the filter to the library

Grep these files for the string `NEWFILTER` to find where you need to make
changes to add your binary filter, and follow the advises given there:
*  [okteta/kasten/controllers/CMakeLists.txt](../../CMakeLists.txt)
*  [okteta/kasten/controllers/view/libbytearrayfilter/bytearrayfilterfactory.cpp](bytearrayfilterfactory.cpp)


### 4. Compile, install and run

Now you are ready to enter the cycle of compile, test, and fix. Do so :)


### 5. Share your results

If you are pleased with your results, contact the maintainer of Okteta, as found
in the "About Okteta" dialog, for inclusion in the official sources.


TODO: A test system to make sure your filter does what it is supposed to.



# How to add a new binary filter with new parameterset

### 0. Create a binary filter with fixed parameters

As a start do the steps 1.-4, as described above and just set the parameters to fixed values,
until the filter basically works.


### 1. Creating source files

Copy the template files   `template_bytearrayfilterparameterset{,edit}.*` from
    [okteta/kasten/controllers/view/libbytearrayfilter/filter/template/](filter/template/)  
to  
    [okteta/kasten/controllers/view/libbytearrayfilter/filter/](filter/)  
using new file names matching the class name.
E.g. for `MyByteArrayFilterParameterSet` use `mybytearrayfilterparameterset{,edit}.*`.


### 2. Adapting the code from the template

In the templates the ranges where you need to do the changes are marked with
```cpp
//// ADAPT(start)
//// ADAPT(end)
```
Follow the advises given at the beginning of each range.


### 3. Adding the parameterset to the library

Grep these files for the string `NEWFILTERPARAMETERSET` to find where you need to make
changes to add your binary filter, and follow the advises given there:
* [okteta/kasten/controllers/CMakeLists.txt](../../CMakeLists.txt)
* [okteta/kasten/controllers/view/libbytearrayfilter/bytearrayfilterparameterseteditfactory.cpp](bytearrayfilterparameterseteditfactory.cpp)

Also change your filter to now make use of the parameter set, by switching from
`NoByteArrayFilterParameterSet` to the one you have created and adapt the `filter()` method
of your filter class to take the actual parameter from the parameter set instead of using
the fixed values as done for the start.


### 4. Compile, install and run

Now you are ready to enter the cycle of compile, test, and fix. Do so :)


### 5. Share your results

If you are pleased with your results, contact the maintainer of Okteta, as found
in the "About Okteta" dialog, for inclusion in the official sources.
