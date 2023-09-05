# AL9000_CLK_CONFIG: for chip pll enable(same as board cfg pin) and call clk_simple_config func
# AL9000_CLK_800M: for apu freq sel, 400, 800, 1000(BUS333M), 1200
# AL9000_OSC_33: for chip select OSC frequency, 25, 33, 50
# AL9000_BRINGUP: for chip bypass mpu disable flow
# BOARD_DR1X90_AD101_V10: for demo board select uart 1 as log output

CFLAGS += -DAL9000_CLK_CONFIG
CFLAGS += -DAL9000_CLK_800M
CFLAGS += -DAL9000_OSC_33
CFLAGS += -DAL9000_BRINGUP
CFLAGS += -DBOARD_DR1X90_AD101_V10
