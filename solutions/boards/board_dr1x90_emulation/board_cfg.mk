# AL_CLK_CONFIG: for chip pll enable(same as board cfg pin) and call clk_simple_config func
# AL_CLK_800M: for apu freq sel, 400(2221), 800(4221), 1000(6221, AHB-BUS333M), 1200(6221); (APU, RPU, AHB, APB)
# AL_OSC_33: for chip select OSC frequency, 25, 33, 50
# AL_BRINGUP: for chip bypass mpu disable flow
# board_dr1x90_emulation: for emulation select uart 0 as log output

CFLAGS += -DAL_CLK_800M
CFLAGS += -DAL_BRINGUP
