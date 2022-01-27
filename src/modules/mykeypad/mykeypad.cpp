#include "mykeypad.h"
#include "util.h"

KeyBind getBindForKey(char keyHex)
{
  int value = hex2int(keyHex);

  if (btnMode.isHeld()){
    if (value < 4) {
      return media_keys[value];
    }
  }

  return keymap[layer][value];
}