# splendor


## WASM <-> JS

### Run application with main infinite loop

First we need to download and install emscripten
https://emscripten.org/docs/getting_started/downloads.html

Then in our C app we need to get rid of the infinite loop and extract it into a function. We set emscripten_main_loop to that function

from :
````
 int main()
 {
	while(1)
	{
		//Do stuff
	}
 }

````
|

|

|

V

````
#include <emscripten.h>

mainLoop()
{
	//do stuff
}


int main()
{
	#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop(mainLoop, 0, 1);
	#endif
	#ifndef __EMSCRIPTEN__
		while(1)
			mainLoop();
	#endif
}
````
### Compiling

instead of gcc we use `emcc`

Then we need to add some compile lines.
IF you use SDl2 you need `-s USE_SDL_TTF=2`
If you have some assets you need to preload them with

`--preload-file your_file_or_folder `
Then in order to have your wasm running as a js module you will need
````
 -s WASM=0 \
		-s EXPORT_ES6=1 \
        -s MODULARIZE=1 \
        -s ENVIRONMENT=web \
        -s ALLOW_MEMORY_GROWTH=1 \
````

the complete compile command should look like

```
emcc srcs/test.c s -I includes
        -s WASM=0 \
		-s EXPORT_ES6=1 \
        -s MODULARIZE=1 \
        -s ENVIRONMENT=web \
        -s ALLOW_MEMORY_GROWTH=1 \
        -o output.js
```

### Running
### Call WASM from js and vice versa

https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-ccall-cwrap

To call to and from JS you will need to add these compile options

```

-s EXPORTED_FUNCTIONS=_main,your_test_fn, \
		-s EXPORTED_RUNTIME_METHODS=ccall,stringToUTF8,lengthBytesUTF8 \
```

To call a JS function from C, you simply need to use `EM_ASM`

````
EM_ASM({test("THIS IS A MESSAGE")});
````

This calls the JS function "test" with the argument "This is a message"


To call a C function from JS you will need to use th ccall method that is available in your Module

````
Module.ccall("function_name", return_type, [arg types], [args]);
````

The arguments are limited to integers, floating point, char * and arrays.

To pass a string, one must first convert it into a char *

To do so we have already exported the runtime functions `stringToUTF8` and `lengthByteUTF8`

We will need to find the length of our string , allocate that much space and then copy ourstring to the allocated space. We cna then pass that pointer to the C function.

````
const byteCount = await Module.lengthBytesUTF8(message) + 1;
		const msgPtr =  Module._malloc(byteCount);
		Module.stringToUTF8(message, msgPtr, byteCount);
		await Module.ccall('test', null, ['string', 'number'], [message, byteCount]);
````

