# AL_CLK_CONFIG: for chip pll enable(same as board cfg pin) and call clk_simple_config func
# AL_CLK_800M: for apu freq sel, 400(2221), 800(4221), 1000(6221, AHB-BUS333M), 1200(6221); (APU, RPU, AHB, APB)
# AL_OSC_33: for chip select OSC frequency, 25, 33, 50
# AL_BRINGUP: for chip bypass mpu disable flow
# BOARD_DR1X90_AD101_V20: for demo board select uart 1 as log output

CFLAGS += -DAL_CLK_CONFIG
ifeq ($(CHIP), dr1m90)
CFLAGS += -DAL_CLK_800M
else
CFLAGS += -DAL_CLK_600M
endif
CFLAGS += -DAL_OSC_33
CFLAGS += -DAL_BRINGUP
CFLAGS += -DBOARD_DR1X90_AD101_V20
