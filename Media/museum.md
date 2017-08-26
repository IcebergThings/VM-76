Project History Museum
======================

Code Fragments
--------------

### SuperXE

```
#define SuperXE(method, pointer) do { \
	switch ((uintptr_t) pointer) { \
	case 0xbaadf00d: \
		(pointer) = NULL; \
		[[fallthrough]]; \
	case 0: \
		break; \
	default: \
		method(pointer); \
		(pointer) = NULL; \
	} \
} while (false)
```
