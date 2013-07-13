## erl-rmmseg

This library binds rmmseg-cpp to erlang as BIF. 

## Compile

Compile all cpp source files in `src/rmmseg-cpp` and link them to a shared library (dll/so). You need install erlang first, and specify the erlang NIF header files to compile.

Once compiled, use functions in `rmmseg` module, i.e:

* load_dicts
* seg_space
* seg


