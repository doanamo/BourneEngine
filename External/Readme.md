# External
This directory contains sources for external third-party libraries. External branch contains unmodified libraries, with some additional instructions/modifications made to e.g. reduce their sizes by removing documentation, binaries, platforms, tests or examples.

All modifications are done on the main branch, so new versions of libraries can be merged more easily from clean external branch. Each modification in code should be tagged in the following way for easier merging:
```
// << BourneEngine
// >> BourneEngine
```

## fmt

* Source: https://github.com/fmtlib/fmt
* Version: [11.2.0](https://github.com/fmtlib/fmt/releases/tag/11.2.0) (40626af88bd7df9a5fb80be7b25ac85b122d6c21)
* Excluded directories: doc, doc-html, test
