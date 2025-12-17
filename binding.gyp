{
  "targets": [
    {
      "target_name": "mynative",
      "sources": [
        "src/cpp/calculator.cc",
        "src/cpp/addon.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ 
        "NAPI_VERSION=6",
        "NAPI_DISABLE_CPP_EXCEPTIONS"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1,
          "AdditionalIncludeDirectories": [
            "<!@(node -p \"require('node-addon-api').include\")"
          ]
        }
      },
      "conditions": [
        ['OS=="win"', {
          "defines": [
            "_HAS_EXCEPTIONS=1",
            "WIN32_LEAN_AND_MEAN"
          ],
          "libraries": [
            "-ladvapi32",
            "-luserenv"
          ]
        }]
      ]
    }
  ]
}