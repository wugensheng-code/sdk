# AL9000_OSC_33: for chip pll enable and call clk_simple_config func
# AL9000_BRINGUP: for chip bypass mpu disable flow
# BOARD_DR1X90_AD101_V10: for demo board select uart 1 as log output

CFLAGS += -DAL9000_BRINGUP -DBOARD_DR1X90_AD101_V10 -DAL9000_OSC_33
