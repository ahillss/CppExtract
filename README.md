# CppExtract

Requires LLVM/CLANG.

Exports C/C++ declarations (enums, structs, unions, classes, functions, methods, contructors, destructors, typedefs and namespaces) and scalar macro defines (strings, numbers and hexadecimals) to XML.

Only tested on Linux, but should be cross platform (the LLVM_PREFIX define might need to be defined).

LLVM/CLANG version last tested on was 3.6.2-1/3.6.2-1.

## Options:
* -I include paths
* -D define macros
* -U undefine macros
* -lang=c,c++ (defaults c++)
* --sxml Output as S-Expr XML
* source1 source2 sourceN...

## TODO:
* handle const types (currently not labelled as such)
* support c++11 (add cli option)
* C++ templates
* 32/64 bit option (for record sizes and field offsets)
* evaluate some macro expressions
* function/method bodies, global variable declarations.

## Example:
* [stb_image.h](tests/stb_image.h)
* [stb_image.xml](tests/stb_image.xml)

```xml
<root>
  <buffer file="/usr/include/stdio.h">
    <define name="_STDIO_H" type="number" value="1" />
  </buffer>
  <buffer file="/usr/include/features.h">
    ...
  </buffer>
  <buffer file="/usr/include/stdc-predef.h">
    ...
  </buffer>
  <buffer file="/usr/include/features.h">
    ...
  </buffer>
  <buffer file="/usr/include/sys/cdefs.h">
    ...
  </buffer>
  <buffer file="/usr/lib/clang/3.6.2/include/stddef.h">
    <typedef name="size_t">
      <builtin name="unsigned long" />
    </typedef>
  </buffer>
  <buffer file="/usr/include/bits/types.h">
    <define name="_BITS_TYPES_H" type="number" value="1" />
  </buffer>
  ...
  <buffer file="tests/stb_image.h">
    <define name="STBI_VERSION" type="number" value="1" />
    <enum>
      <item name="STBI_default" value="0" />
      <item name="STBI_grey" value="1" />
      <item name="STBI_grey_alpha" value="2" />
      <item name="STBI_rgb" value="3" />
      <item name="STBI_rgb_alpha" value="4" />
    </enum>
    <typedef name="stbi_uc">
      <builtin name="unsigned char" />
    </typedef>
    <struct name="41954144" size="24">
      <field name="read" offset="0">
        <pointer>
          <function>
            <return>
              <builtin name="int" />
            </return>
            <param>
              <pointer>
                <builtin name="void" />
              </pointer>
            </param>
            <param>
              <pointer>
                <builtin name="char" />
              </pointer>
            </param>
            <param>
              <builtin name="int" />
            </param>
          </function>
        </pointer>
      </field>
      <field name="skip" offset="8">
        <pointer>
          <function>
            <return>
              <builtin name="void" />
            </return>
            <param>
              <pointer>
                <builtin name="void" />
              </pointer>
            </param>
            <param>
              <builtin name="int" />
            </param>
          </function>
        </pointer>
      </field>
      <field name="eof" offset="16">
        <pointer>
          <function>
            <return>
              <builtin name="int" />
            </return>
            <param>
              <pointer>
                <builtin name="void" />
              </pointer>
            </param>
          </function>
        </pointer>
      </field>
    </struct>
    <typedef name="stbi_io_callbacks">
      <struct name="41954144" />
    </typedef>
    ...
    <function name="stbi_zlib_decode_noheader_malloc">
      <return>
        <pointer>
          <builtin name="char" />
        </pointer>
      </return>
      <param name="buffer">
        <pointer>
          <builtin name="char" />
        </pointer>
      </param>
      <param name="len">
        <builtin name="int" />
      </param>
      <param name="outlen">
        <pointer>
          <builtin name="int" />
        </pointer>
      </param>
    </function>
    <function name="stbi_zlib_decode_noheader_buffer">
      <return>
        <builtin name="int" />
      </return>
      <param name="obuffer">
        <pointer>
          <builtin name="char" />
        </pointer>
      </param>
      <param name="olen">
        <builtin name="int" />
      </param>
      <param name="ibuffer">
        <pointer>
          <builtin name="char" />
        </pointer>
      </param>
      <param name="ilen">
        <builtin name="int" />
      </param>
    </function>
  </buffer>
</root>
```
