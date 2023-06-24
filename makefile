# ----------------------------
# Makefile Options
# ----------------------------

NAME = Zelda
ICON = icon.png
DESCRIPTION = "NES Zelda for TI84CE"
COMPRESSED = NO
ARCHIVED = YES
OUTPUT_MAP = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
