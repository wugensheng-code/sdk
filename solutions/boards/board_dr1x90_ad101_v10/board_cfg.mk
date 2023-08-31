# AL9000_CLK_CONFIG: for chip pll enable(same as board cfg pin) and call clk_simple_config func(2:2:2:1)
# AL9000_OSC_33: for chip select OSC frequency
# AL9000_BRINGUP: for chip bypass mpu disable flow
# BOARD_DR1X90_AD101_V10: for demo board select uart 1 as log output

CFLAGS += -DAL9000_CLK_CONFIG
CFLAGS += -DAL9000_OSC_33
CFLAGS += -DAL9000_BRINGUP
CFLAGS += -DBOARD_DR1X90_AD101_V10
