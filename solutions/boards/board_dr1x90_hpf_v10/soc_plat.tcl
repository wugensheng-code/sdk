init
source [find mem_helper.tcl]
proc mask_write {addr mask val } {
	set old_val [mrw $addr]
	set new_val [expr {($val & $mask) | ($old_val & (~$mask))}]
	mww $addr $new_val
}


proc write {addr val } {
	set old_val [mrw $addr]
	set new_val $val
	mww $addr $new_val
}

set POLL_MAX_TIMES 1000
proc mask_poll {addr mask val } {
	set index 0
	set old [mrw $addr]
	set flag [expr {$old & $mask}]
	while { $flag != $val} { 
		if { $index == $POLL_MAX_TIMES } { 
			puts "Info : mask poll timeout: " $addr
			break 
		}
		set old [mrw $addr]
		set flag [expr {$old & $mask}]
		set index = $index + 1
	}	
}

# config ddr using elf start -->
proc wait_ddr_init {} {
	set t [target current]
	for {set j 0} {$j < 10} {incr j}  {
		halt
		set DFISTAT [expr {[mrw 0xf84211bc] & 0x1}]
		set STAT	[expr {[mrw 0xf8421004] & 0x7}]

		if {($DFISTAT == 1) && ($STAT != 0)} {
			puts "Info : DDR initialization: Completed"
			break
		} else {
			resume
			puts "Info : DDR initialization... "
			sleep 100
		}
	}
	if {$j > 8} {
		puts "Error : DDR initialization: Failed"
	}
	halt
}

set ROW_COL_BANK                    0
set ROW_BANK_COL                    1
set BANK_ROW_COL                    2

set DDR4                            4
set DDR3                            3

set Internal                        0
set External                        1

set x16                            16
set x8                              8

set DDR3_800D                       0
set DDR3_800E                       1
set DDR3_1066E                      2
set DDR3_1066F                      3
set DDR3_1066G                      4
set DDR3_1333F                      5
set DDR3_1333G                      6
set DDR3_1333H                      7
set DDR3_1333J                      8
set DDR4_1600J                      9
set DDR4_1600K                     10
set DDR4_1600L                     11

set NONE                            0
set WDM                             1
set RDBI                            1

set AARCH64                         0
set RISCV                           1
set p_arch                   $AARCH64

set CAN0_SLOW_RATE            1000000
set CAN0_TYPE              0x40000000
set DDR_ENABLE                      1
set DDRC_ADDR_MAP       $ROW_COL_BANK
set DDRC_COMP_DENSITY            4096
set DDRC_SPEED_BIN        $DDR3_1066G
set DDRC_COMP_TYPE               $x16
set DDRC_IO_VOL            0x3FC00000
set DDRC_PZQ_CONF                   0
set DDRC_RDBI                       0
set DDRC_DDR_SPEED               1066
set DDRC_TRAINING_EN                1
set DDRC_DELAY_B0_AC                0
set DDRC_DELAY_B0_DQ                0
set DDRC_DDR_TYPE               $DDR3
set DDRC_VREF               $Internal
set DDRC_WDM_WDBI               $NONE
set DDRC_DQ_WIDTH                  32
set I2C0_RATE              0x3DCCCCCD
set OSC_CLK                  33330000
set UART1_BAUDRATE             115200

set p_osc_clk          33330000
set p_type             3       
set p_speed            1066    
set p_dq_width         32      
set p_io_vol           1    
set p_verf             0       
set p_pzq              0       
set p_dram_width       8       
set p_speed_bin_index  4       
set p_wdbi             1       
set p_rdbi             0       
set p_dram_density     4096    
set p_ecc              0       
set p_addr_map         0       
set p_training         1       
set p_byte0_ac_dely    0     
set p_byte0_dqs_dely   0     
set p_byte1_ac_dely    0     
set p_byte1_dqs_dely   0     
set p_byte2_ac_dely    0     
set p_byte2_dqs_dely   0 
set p_byte3_ac_dely    0
set p_byte3_dqs_dely   0 

# (1)
if {[info exists DDRC_ADDR_MAP]} {
	set p_addr_map $DDRC_ADDR_MAP
} 

# (2)
if {[info exists DDRC_COMP_DENSITY]} {
	set p_dram_density $DDRC_COMP_DENSITY
} 

# (3)
if {[info exists DDRC_SPEED_BIN]} {
	set p_speed_bin_index $DDRC_SPEED_BIN
} 

# (4)
if {[info exists DDRC_COMP_TYPE]} {
	set p_dram_width $DDRC_COMP_TYPE
} 

# (5)
if {[info exists DDRC_IO_VOL]} {
	set p_io_vol $DDRC_IO_VOL
} 

# (6)
if {[info exists DDRC_PZQ_CONF]} {
	set p_pzq $DDRC_PZQ_CONF
} 

# (7)
if {[info exists DDRC_DDR_SPEED]} {
	set p_speed $DDRC_DDR_SPEED
} 

# (8)
if {[info exists DDRC_TRAINING_EN]} {
	set p_training $DDRC_TRAINING_EN
} 

# (9)
if {[info exists DDRC_DELAY_B0_AC]} {
	set p_byte0_ac_dely $DDRC_DELAY_B0_AC
} 

# (10)
if {[info exists DDRC_DELAY_B0_DQ]} {
	set p_byte0_dqs_dely $DDRC_DELAY_B0_DQ
} 

# (11)
if {[info exists DDRC_DELAY_B1_AC]} {
	set p_byte1_ac_dely $DDRC_DELAY_B1_AC
} 

# (12)
if {[info exists DDRC_DELAY_B1_DQ]} {
	set p_byte1_dqs_dely $DDRC_DELAY_B1_DQ
} 

# (13)
if {[info exists DDRC_DELAY_B2_AC]} {
	set p_byte2_ac_dely $DDRC_DELAY_B2_AC
} 

# (14)
if {[info exists DDRC_DELAY_B2_DQ]} {
	set p_byte2_dqs_dely $DDRC_DELAY_B2_DQ
} 

# (15)
if {[info exists DDRC_DELAY_B3_AC]} {
	set p_byte3_ac_dely $DDRC_DELAY_B3_AC
} 

# (16)
if {[info exists DDRC_DELAY_B3_DQ]} {
	set p_byte3_dqs_dely $DDRC_DELAY_B3_DQ
} 

# (17)
if {[info exists DDRC_DDR_TYPE]} {
	set p_type $DDRC_DDR_TYPE
} 

# (18)
if {[info exists DDRC_VREF]} {
	set p_verf $DDRC_VREF
} 

# (19)
if {[info exists DDRC_WDM_WDBI]} {
	set p_wdbi $DDRC_WDM_WDBI
} 

# (20)
if {[info exists DDRC_DQ_WIDTH]} {
	set p_dq_width $DDRC_DQ_WIDTH
} 

# (21)
if {[info exists OSC_CLK]} {
	set p_osc_clk $OSC_CLK
} 

# (22)
if {[info exists DDRC_RDBI]} {
	set p_rdbi $DDRC_RDBI
} 

# (23)
if {[info exists DDRC_ECC_EN]} {
	set p_ecc $DDRC_ECC_EN
} 


proc ddr_init { } {
	set DFISTAT [expr {[mrw 0xf84211bc] & 0x1}]
	set STAT	[expr {[mrw 0xf8421004] & 0x7}]

	if {($DFISTAT == 1) && ($STAT != 0)} {
		puts "info : Skip, DDR is ready."
	} else {
		puts "info : Load DDR initialization code."
		load_image $::env(FD_ROOT_PATH)/tools/data/dr1m90_ddr_init.elf
		mww 0x61030000 $::p_osc_clk
		mww 0x61030004 $::p_type
		mww 0x61030008 $::p_speed
		mww 0x6103000c $::p_dq_width 
		mww 0x61030010 $::p_io_vol  
		mww 0x61030014 $::p_verf
		mww 0x61030018 $::p_pzq
		mww 0x6103001c $::p_dram_width
		mww 0x61030020 $::p_speed_bin_index
		mww 0x61030024 $::p_wdbi
		mww 0x61030028 $::p_rdbi
		mww 0x6103002c $::p_dram_density
		mww 0x61030030 $::p_ecc
		mww 0x61030034 $::p_addr_map
		mww 0x61030038 $::p_training
		mww 0x6103003c $::p_byte0_ac_dely
		mww 0x61030040 $::p_byte0_dqs_dely
		mww 0x61030044 $::p_byte1_ac_dely
		mww 0x61030048 $::p_byte1_dqs_dely
		mww 0x6103004c $::p_byte2_ac_dely
		mww 0x61030050 $::p_byte2_dqs_dely
		mww 0x61030054 $::p_byte3_ac_dely
		mww 0x61030058 $::p_byte3_dqs_dely
		mww 0x6103005c $::p_arch

		resume 0x61000000
		wait_ddr_init
	}
}
# config ddr using elf end <--

# (0) config system
proc config_system { } {
	# Config pin, fix to LVCMOS 1.8V
	# register: IO_BANK200_REF.bank200_mc1_force_dtc = 0x1
	# 0xF8803c00[5:4] = 0x1
	mask_write 0xF8803c00 0x00000030 0x00000010

	# Config pin, fix to LVCMOS 1.8V
	# register: IO_BANK201_REF.bank201_mc1_force_dtc = 0x1
	# 0xF8803c04[5:4] = 0x1
	mask_write 0xF8803c04 0x00000030 0x00000010

	# Config POWER_MODE, Set npu power mode,0:SLP,2:SD
	# register: POWER_MODE.power_mode_npu = 0x2
	# 0xF8800030[29:24] = 0x2
	mask_write 0xF8800030 0x3f000000 0x02000000

	# Config NPU, status: Disable
	# register: CFG_CTRL_NPU.ctrl_npu = 0x0
	# 0xF88001BC[1:0] = 0x0
	mask_write 0xF88001BC 0x00000003 0x00000000

	# Config JTAG, RV TAP dummy
	# register: JTAG_CTRL.rv_tap_off = 0x1
	# 0xF88060A8[2:2] = 0x1
	mask_write 0xF88060A8 0x00000004 0x00000004

	# Config POWER_MODE, Set rpu power mode:,0:SLP,2:SD
	# register: POWER_MODE.power_mode_rpu = 0x2
	# 0xF8800030[21:16] = 0x2
	mask_write 0xF8800030 0x003f0000 0x00020000

}

# (1) config psio
proc config_psio { } {
	# Config CAN0_tx, PS IO 47->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_47.mc1_ensrc = 0x06
	# 0xf880397c[12:8] = 0x06
	mask_write 0xf880397c 0x00001f00 0x00000600

	# Config v, PS IO 47->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_47.mc1_i_mode = 0x2
	# 0xf880397c[1:0] = 0x2
	mask_write 0xf880397c 0x00000003 0x00000002

	# Config CAN0_rx, PS IO 46->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_46.mc1_ensrc = 0x06
	# 0xf8803974[12:8] = 0x06
	mask_write 0xf8803974 0x00001f00 0x00000600

	# Config v, PS IO 46->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_46.mc1_i_mode = 0x2
	# 0xf8803974[1:0] = 0x2
	mask_write 0xf8803974 0x00000003 0x00000002

	# Config CAN0_tx, PS IO 47->speed: slow
	# register: PARA0_PS_MIO_47.speed = 0x0
	# 0xf8803978[2:0] = 0x0
	mask_write 0xf8803978 0x00000007 0x00000000

	# Config CAN0_tx, PS IO 47->PullType: pullup 25k
	# register: PARA0_PS_MIO_47.PullType = 0x8
	# 0xf8803978[31:28] = 0x8
	mask_write 0xf8803978 0xf0000000 0x80000000

	# Config CAN0_rx, PS IO 46->speed: slow
	# register: PARA0_PS_MIO_46.speed = 0x0
	# 0xf8803970[2:0] = 0x0
	mask_write 0xf8803970 0x00000007 0x00000000

	# Config CAN0_rx, PS IO 46->PullType: pullup 25k
	# register: PARA0_PS_MIO_46.PullType = 0x8
	# 0xf8803970[31:28] = 0x8
	mask_write 0xf8803970 0xf0000000 0x80000000

	# Config CAN0, PS IO 47->FUNCSEL: CAN0
	# register: FUNCSEL_PS_MIO_47.funcsel_ps_mio_47 = 1
	# 0xf88030bc[3:0] = 1
	mask_write 0xf88030bc 0x0000000f 0x00000001

	# Config CAN0, PS IO 46->FUNCSEL: CAN0
	# register: FUNCSEL_PS_MIO_46.funcsel_ps_mio_46 = 1
	# 0xf88030b8[3:0] = 1
	mask_write 0xf88030b8 0x0000000f 0x00000001

	# Config CAN0, Select IO: "PS IO 46...47", emio selection: mio, as a default value
	# register: EMIOSEL_2.emiosel_2 = 1
	# 0xF8803408[0:0] = 1
	mask_write 0xF8803408 0x00000001 0x00000001

	# Config ETH0_tx_clk, PS IO 16->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_16.mc1_ensrc = 0x15
	# 0xf8803884[12:8] = 0x15
	mask_write 0xf8803884 0x00001f00 0x00001500

	# Config v, PS IO 16->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_16.mc1_i_mode = 0x2
	# 0xf8803884[1:0] = 0x2
	mask_write 0xf8803884 0x00000003 0x00000002

	# Config ETH0_tx_ctl, PS IO 21->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_21.mc1_ensrc = 0x15
	# 0xf88038ac[12:8] = 0x15
	mask_write 0xf88038ac 0x00001f00 0x00001500

	# Config v, PS IO 21->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_21.mc1_i_mode = 0x2
	# 0xf88038ac[1:0] = 0x2
	mask_write 0xf88038ac 0x00000003 0x00000002

	# Config ETH0_txd3, PS IO 20->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_20.mc1_ensrc = 0x15
	# 0xf88038a4[12:8] = 0x15
	mask_write 0xf88038a4 0x00001f00 0x00001500

	# Config v, PS IO 20->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_20.mc1_i_mode = 0x2
	# 0xf88038a4[1:0] = 0x2
	mask_write 0xf88038a4 0x00000003 0x00000002

	# Config ETH0_txd2, PS IO 19->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_19.mc1_ensrc = 0x15
	# 0xf880389c[12:8] = 0x15
	mask_write 0xf880389c 0x00001f00 0x00001500

	# Config v, PS IO 19->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_19.mc1_i_mode = 0x2
	# 0xf880389c[1:0] = 0x2
	mask_write 0xf880389c 0x00000003 0x00000002

	# Config ETH0_txd1, PS IO 18->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_18.mc1_ensrc = 0x15
	# 0xf8803894[12:8] = 0x15
	mask_write 0xf8803894 0x00001f00 0x00001500

	# Config v, PS IO 18->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_18.mc1_i_mode = 0x2
	# 0xf8803894[1:0] = 0x2
	mask_write 0xf8803894 0x00000003 0x00000002

	# Config ETH0_txd0, PS IO 17->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_17.mc1_ensrc = 0x15
	# 0xf880388c[12:8] = 0x15
	mask_write 0xf880388c 0x00001f00 0x00001500

	# Config v, PS IO 17->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_17.mc1_i_mode = 0x2
	# 0xf880388c[1:0] = 0x2
	mask_write 0xf880388c 0x00000003 0x00000002

	# Config ETH0_rx_clk, PS IO 22->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_22.mc1_ensrc = 0x15
	# 0xf88038b4[12:8] = 0x15
	mask_write 0xf88038b4 0x00001f00 0x00001500

	# Config v, PS IO 22->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_22.mc1_i_mode = 0x2
	# 0xf88038b4[1:0] = 0x2
	mask_write 0xf88038b4 0x00000003 0x00000002

	# Config ETH0_rx_ctl, PS IO 27->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_27.mc1_ensrc = 0x15
	# 0xf88038dc[12:8] = 0x15
	mask_write 0xf88038dc 0x00001f00 0x00001500

	# Config v, PS IO 27->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_27.mc1_i_mode = 0x2
	# 0xf88038dc[1:0] = 0x2
	mask_write 0xf88038dc 0x00000003 0x00000002

	# Config ETH0_rxd3, PS IO 26->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_26.mc1_ensrc = 0x15
	# 0xf88038d4[12:8] = 0x15
	mask_write 0xf88038d4 0x00001f00 0x00001500

	# Config v, PS IO 26->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_26.mc1_i_mode = 0x2
	# 0xf88038d4[1:0] = 0x2
	mask_write 0xf88038d4 0x00000003 0x00000002

	# Config ETH0_rxd2, PS IO 25->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_25.mc1_ensrc = 0x15
	# 0xf88038cc[12:8] = 0x15
	mask_write 0xf88038cc 0x00001f00 0x00001500

	# Config v, PS IO 25->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_25.mc1_i_mode = 0x2
	# 0xf88038cc[1:0] = 0x2
	mask_write 0xf88038cc 0x00000003 0x00000002

	# Config ETH0_rxd1, PS IO 24->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_24.mc1_ensrc = 0x15
	# 0xf88038c4[12:8] = 0x15
	mask_write 0xf88038c4 0x00001f00 0x00001500

	# Config v, PS IO 24->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_24.mc1_i_mode = 0x2
	# 0xf88038c4[1:0] = 0x2
	mask_write 0xf88038c4 0x00000003 0x00000002

	# Config ETH0_rxd0, PS IO 23->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_23.mc1_ensrc = 0x15
	# 0xf88038bc[12:8] = 0x15
	mask_write 0xf88038bc 0x00001f00 0x00001500

	# Config v, PS IO 23->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_23.mc1_i_mode = 0x2
	# 0xf88038bc[1:0] = 0x2
	mask_write 0xf88038bc 0x00000003 0x00000002

	# Config ETH0_tx_clk, PS IO 16->speed: fast
	# register: PARA0_PS_MIO_16.speed = 0x7
	# 0xf8803880[2:0] = 0x7
	mask_write 0xf8803880 0x00000007 0x00000007

	# Config ETH0_tx_clk, PS IO 16->PullType: pullup 25k
	# register: PARA0_PS_MIO_16.PullType = 0x8
	# 0xf8803880[31:28] = 0x8
	mask_write 0xf8803880 0xf0000000 0x80000000

	# Config ETH0_tx_ctl, PS IO 21->speed: fast
	# register: PARA0_PS_MIO_21.speed = 0x7
	# 0xf88038a8[2:0] = 0x7
	mask_write 0xf88038a8 0x00000007 0x00000007

	# Config ETH0_tx_ctl, PS IO 21->PullType: pullup 25k
	# register: PARA0_PS_MIO_21.PullType = 0x8
	# 0xf88038a8[31:28] = 0x8
	mask_write 0xf88038a8 0xf0000000 0x80000000

	# Config ETH0_txd3, PS IO 20->speed: fast
	# register: PARA0_PS_MIO_20.speed = 0x7
	# 0xf88038a0[2:0] = 0x7
	mask_write 0xf88038a0 0x00000007 0x00000007

	# Config ETH0_txd3, PS IO 20->PullType: pullup 25k
	# register: PARA0_PS_MIO_20.PullType = 0x8
	# 0xf88038a0[31:28] = 0x8
	mask_write 0xf88038a0 0xf0000000 0x80000000

	# Config ETH0_txd2, PS IO 19->speed: fast
	# register: PARA0_PS_MIO_19.speed = 0x7
	# 0xf8803898[2:0] = 0x7
	mask_write 0xf8803898 0x00000007 0x00000007

	# Config ETH0_txd2, PS IO 19->PullType: pullup 25k
	# register: PARA0_PS_MIO_19.PullType = 0x8
	# 0xf8803898[31:28] = 0x8
	mask_write 0xf8803898 0xf0000000 0x80000000

	# Config ETH0_txd1, PS IO 18->speed: fast
	# register: PARA0_PS_MIO_18.speed = 0x7
	# 0xf8803890[2:0] = 0x7
	mask_write 0xf8803890 0x00000007 0x00000007

	# Config ETH0_txd1, PS IO 18->PullType: pullup 25k
	# register: PARA0_PS_MIO_18.PullType = 0x8
	# 0xf8803890[31:28] = 0x8
	mask_write 0xf8803890 0xf0000000 0x80000000

	# Config ETH0_txd0, PS IO 17->speed: fast
	# register: PARA0_PS_MIO_17.speed = 0x7
	# 0xf8803888[2:0] = 0x7
	mask_write 0xf8803888 0x00000007 0x00000007

	# Config ETH0_txd0, PS IO 17->PullType: pullup 25k
	# register: PARA0_PS_MIO_17.PullType = 0x8
	# 0xf8803888[31:28] = 0x8
	mask_write 0xf8803888 0xf0000000 0x80000000

	# Config ETH0_rx_clk, PS IO 22->speed: fast
	# register: PARA0_PS_MIO_22.speed = 0x7
	# 0xf88038b0[2:0] = 0x7
	mask_write 0xf88038b0 0x00000007 0x00000007

	# Config ETH0_rx_clk, PS IO 22->PullType: pullup 25k
	# register: PARA0_PS_MIO_22.PullType = 0x8
	# 0xf88038b0[31:28] = 0x8
	mask_write 0xf88038b0 0xf0000000 0x80000000

	# Config v, PS IO 22->SignalHysteresis: LVCMOS 1.8V
	# register: PARA0_PS_MIO_22.SignalHysteresis = 0x8
	# 0xf88038b0[27:24] = 0x8
	mask_write 0xf88038b0 0x0f000000 0x08000000

	# Config ETH0_rx_ctl, PS IO 27->speed: fast
	# register: PARA0_PS_MIO_27.speed = 0x7
	# 0xf88038d8[2:0] = 0x7
	mask_write 0xf88038d8 0x00000007 0x00000007

	# Config ETH0_rx_ctl, PS IO 27->PullType: pullup 25k
	# register: PARA0_PS_MIO_27.PullType = 0x8
	# 0xf88038d8[31:28] = 0x8
	mask_write 0xf88038d8 0xf0000000 0x80000000

	# Config v, PS IO 27->SignalHysteresis: LVCMOS 1.8V
	# register: PARA0_PS_MIO_27.SignalHysteresis = 0x8
	# 0xf88038d8[27:24] = 0x8
	mask_write 0xf88038d8 0x0f000000 0x08000000

	# Config ETH0_rxd3, PS IO 26->speed: fast
	# register: PARA0_PS_MIO_26.speed = 0x7
	# 0xf88038d0[2:0] = 0x7
	mask_write 0xf88038d0 0x00000007 0x00000007

	# Config ETH0_rxd3, PS IO 26->PullType: pullup 25k
	# register: PARA0_PS_MIO_26.PullType = 0x8
	# 0xf88038d0[31:28] = 0x8
	mask_write 0xf88038d0 0xf0000000 0x80000000

	# Config v, PS IO 26->SignalHysteresis: LVCMOS 1.8V
	# register: PARA0_PS_MIO_26.SignalHysteresis = 0x8
	# 0xf88038d0[27:24] = 0x8
	mask_write 0xf88038d0 0x0f000000 0x08000000

	# Config ETH0_rxd2, PS IO 25->speed: fast
	# register: PARA0_PS_MIO_25.speed = 0x7
	# 0xf88038c8[2:0] = 0x7
	mask_write 0xf88038c8 0x00000007 0x00000007

	# Config ETH0_rxd2, PS IO 25->PullType: pullup 25k
	# register: PARA0_PS_MIO_25.PullType = 0x8
	# 0xf88038c8[31:28] = 0x8
	mask_write 0xf88038c8 0xf0000000 0x80000000

	# Config v, PS IO 25->SignalHysteresis: LVCMOS 1.8V
	# register: PARA0_PS_MIO_25.SignalHysteresis = 0x8
	# 0xf88038c8[27:24] = 0x8
	mask_write 0xf88038c8 0x0f000000 0x08000000

	# Config ETH0_rxd1, PS IO 24->speed: fast
	# register: PARA0_PS_MIO_24.speed = 0x7
	# 0xf88038c0[2:0] = 0x7
	mask_write 0xf88038c0 0x00000007 0x00000007

	# Config ETH0_rxd1, PS IO 24->PullType: pullup 25k
	# register: PARA0_PS_MIO_24.PullType = 0x8
	# 0xf88038c0[31:28] = 0x8
	mask_write 0xf88038c0 0xf0000000 0x80000000

	# Config v, PS IO 24->SignalHysteresis: LVCMOS 1.8V
	# register: PARA0_PS_MIO_24.SignalHysteresis = 0x8
	# 0xf88038c0[27:24] = 0x8
	mask_write 0xf88038c0 0x0f000000 0x08000000

	# Config ETH0_rxd0, PS IO 23->speed: fast
	# register: PARA0_PS_MIO_23.speed = 0x7
	# 0xf88038b8[2:0] = 0x7
	mask_write 0xf88038b8 0x00000007 0x00000007

	# Config ETH0_rxd0, PS IO 23->PullType: pullup 25k
	# register: PARA0_PS_MIO_23.PullType = 0x8
	# 0xf88038b8[31:28] = 0x8
	mask_write 0xf88038b8 0xf0000000 0x80000000

	# Config v, PS IO 23->SignalHysteresis: LVCMOS 1.8V
	# register: PARA0_PS_MIO_23.SignalHysteresis = 0x8
	# 0xf88038b8[27:24] = 0x8
	mask_write 0xf88038b8 0x0f000000 0x08000000

	# Config ETH0, PS IO 16->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_16.funcsel_ps_mio_16 = 14
	# 0xf8803040[3:0] = 14
	mask_write 0xf8803040 0x0000000f 0x0000000e

	# Config ETH0, PS IO 21->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_21.funcsel_ps_mio_21 = 14
	# 0xf8803054[3:0] = 14
	mask_write 0xf8803054 0x0000000f 0x0000000e

	# Config ETH0, PS IO 20->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_20.funcsel_ps_mio_20 = 14
	# 0xf8803050[3:0] = 14
	mask_write 0xf8803050 0x0000000f 0x0000000e

	# Config ETH0, PS IO 19->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_19.funcsel_ps_mio_19 = 14
	# 0xf880304c[3:0] = 14
	mask_write 0xf880304c 0x0000000f 0x0000000e

	# Config ETH0, PS IO 18->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_18.funcsel_ps_mio_18 = 14
	# 0xf8803048[3:0] = 14
	mask_write 0xf8803048 0x0000000f 0x0000000e

	# Config ETH0, PS IO 17->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_17.funcsel_ps_mio_17 = 14
	# 0xf8803044[3:0] = 14
	mask_write 0xf8803044 0x0000000f 0x0000000e

	# Config ETH0, PS IO 22->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_22.funcsel_ps_mio_22 = 14
	# 0xf8803058[3:0] = 14
	mask_write 0xf8803058 0x0000000f 0x0000000e

	# Config ETH0, PS IO 27->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_27.funcsel_ps_mio_27 = 14
	# 0xf880306c[3:0] = 14
	mask_write 0xf880306c 0x0000000f 0x0000000e

	# Config ETH0, PS IO 26->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_26.funcsel_ps_mio_26 = 14
	# 0xf8803068[3:0] = 14
	mask_write 0xf8803068 0x0000000f 0x0000000e

	# Config ETH0, PS IO 25->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_25.funcsel_ps_mio_25 = 14
	# 0xf8803064[3:0] = 14
	mask_write 0xf8803064 0x0000000f 0x0000000e

	# Config ETH0, PS IO 24->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_24.funcsel_ps_mio_24 = 14
	# 0xf8803060[3:0] = 14
	mask_write 0xf8803060 0x0000000f 0x0000000e

	# Config ETH0, PS IO 23->FUNCSEL: ETH0
	# register: FUNCSEL_PS_MIO_23.funcsel_ps_mio_23 = 14
	# 0xf880305c[3:0] = 14
	mask_write 0xf880305c 0x0000000f 0x0000000e

	# Config GPIO_PS_IO0, PS IO 0->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_0.mc1_ensrc = 0x06
	# 0xf8803804[12:8] = 0x06
	mask_write 0xf8803804 0x00001f00 0x00000600

	# Config v, PS IO 0->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_0.mc1_i_mode = 0x2
	# 0xf8803804[1:0] = 0x2
	mask_write 0xf8803804 0x00000003 0x00000002

	# Config GPIO_PS_IO7, PS IO 7->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_7.mc1_ensrc = 0x06
	# 0xf880383c[12:8] = 0x06
	mask_write 0xf880383c 0x00001f00 0x00000600

	# Config v, PS IO 7->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_7.mc1_i_mode = 0x2
	# 0xf880383c[1:0] = 0x2
	mask_write 0xf880383c 0x00000003 0x00000002

	# Config GPIO_PS_IO8, PS IO 8->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_8.mc1_ensrc = 0x06
	# 0xf8803844[12:8] = 0x06
	mask_write 0xf8803844 0x00001f00 0x00000600

	# Config v, PS IO 8->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_8.mc1_i_mode = 0x2
	# 0xf8803844[1:0] = 0x2
	mask_write 0xf8803844 0x00000003 0x00000002

	# Config GPIO_PS_IO9, PS IO 9->Single-ended pull down Drive Strength: 8
	# register: PARA1_PS_MIO_9.mc1_ensnk = 0x1E
	# 0xf880384c[20:16] = 0x1E
	mask_write 0xf880384c 0x001f0000 0x001e0000

	# Config v, PS IO 9->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_9.mc1_i_mode = 0x2
	# 0xf880384c[1:0] = 0x2
	mask_write 0xf880384c 0x00000003 0x00000002

	# Config GPIO_PS_IO14, PS IO 14->Single-ended pull down Drive Strength: 8
	# register: PARA1_PS_MIO_14.mc1_ensnk = 0x1E
	# 0xf8803874[20:16] = 0x1E
	mask_write 0xf8803874 0x001f0000 0x001e0000

	# Config v, PS IO 14->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_14.mc1_i_mode = 0x2
	# 0xf8803874[1:0] = 0x2
	mask_write 0xf8803874 0x00000003 0x00000002

	# Config GPIO_PS_IO15, PS IO 15->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_15.mc1_ensrc = 0x06
	# 0xf880387c[12:8] = 0x06
	mask_write 0xf880387c 0x00001f00 0x00000600

	# Config v, PS IO 15->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_15.mc1_i_mode = 0x2
	# 0xf880387c[1:0] = 0x2
	mask_write 0xf880387c 0x00000003 0x00000002

	# Config GPIO_PS_IO0, PS IO 0->speed: slow
	# register: PARA0_PS_MIO_0.speed = 0x0
	# 0xf8803800[2:0] = 0x0
	mask_write 0xf8803800 0x00000007 0x00000000

	# Config GPIO_PS_IO0, PS IO 0->PullType: pullup 25k
	# register: PARA0_PS_MIO_0.PullType = 0x8
	# 0xf8803800[31:28] = 0x8
	mask_write 0xf8803800 0xf0000000 0x80000000

	# Config GPIO_PS_IO7, PS IO 7->speed: slow
	# register: PARA0_PS_MIO_7.speed = 0x0
	# 0xf8803838[2:0] = 0x0
	mask_write 0xf8803838 0x00000007 0x00000000

	# Config GPIO_PS_IO7, PS IO 7->PullType: pullup 25k
	# register: PARA0_PS_MIO_7.PullType = 0x8
	# 0xf8803838[31:28] = 0x8
	mask_write 0xf8803838 0xf0000000 0x80000000

	# Config GPIO_PS_IO8, PS IO 8->speed: slow
	# register: PARA0_PS_MIO_8.speed = 0x0
	# 0xf8803840[2:0] = 0x0
	mask_write 0xf8803840 0x00000007 0x00000000

	# Config GPIO_PS_IO8, PS IO 8->PullType: pullup 25k
	# register: PARA0_PS_MIO_8.PullType = 0x8
	# 0xf8803840[31:28] = 0x8
	mask_write 0xf8803840 0xf0000000 0x80000000

	# Config GPIO_PS_IO9, PS IO 9->speed: slow
	# register: PARA0_PS_MIO_9.speed = 0x0
	# 0xf8803848[2:0] = 0x0
	mask_write 0xf8803848 0x00000007 0x00000000

	# Config GPIO_PS_IO9, PS IO 9->PullType: pulldown 25k
	# register: PARA0_PS_MIO_9.PullType = 0x4
	# 0xf8803848[31:28] = 0x4
	mask_write 0xf8803848 0xf0000000 0x40000000

	# Config GPIO_PS_IO14, PS IO 14->speed: slow
	# register: PARA0_PS_MIO_14.speed = 0x0
	# 0xf8803870[2:0] = 0x0
	mask_write 0xf8803870 0x00000007 0x00000000

	# Config GPIO_PS_IO14, PS IO 14->PullType: pulldown 25k
	# register: PARA0_PS_MIO_14.PullType = 0x4
	# 0xf8803870[31:28] = 0x4
	mask_write 0xf8803870 0xf0000000 0x40000000

	# Config GPIO_PS_IO15, PS IO 15->speed: slow
	# register: PARA0_PS_MIO_15.speed = 0x0
	# 0xf8803878[2:0] = 0x0
	mask_write 0xf8803878 0x00000007 0x00000000

	# Config GPIO_PS_IO15, PS IO 15->PullType: pullup 25k
	# register: PARA0_PS_MIO_15.PullType = 0x8
	# 0xf8803878[31:28] = 0x8
	mask_write 0xf8803878 0xf0000000 0x80000000

	# Config GPIO, PS IO 0->FUNCSEL: GPIO
	# register: FUNCSEL_PS_MIO_0.funcsel_ps_mio_0 = 4
	# 0xf8803000[3:0] = 4
	mask_write 0xf8803000 0x0000000f 0x00000004

	# Config GPIO, PS IO 7->FUNCSEL: GPIO
	# register: FUNCSEL_PS_MIO_7.funcsel_ps_mio_7 = 4
	# 0xf880301c[3:0] = 4
	mask_write 0xf880301c 0x0000000f 0x00000004

	# Config GPIO, PS IO 8->FUNCSEL: GPIO
	# register: FUNCSEL_PS_MIO_8.funcsel_ps_mio_8 = 4
	# 0xf8803020[3:0] = 4
	mask_write 0xf8803020 0x0000000f 0x00000004

	# Config GPIO, PS IO 9->FUNCSEL: GPIO
	# register: FUNCSEL_PS_MIO_9.funcsel_ps_mio_9 = 4
	# 0xf8803024[3:0] = 4
	mask_write 0xf8803024 0x0000000f 0x00000004

	# Config GPIO, PS IO 14->FUNCSEL: GPIO
	# register: FUNCSEL_PS_MIO_14.funcsel_ps_mio_14 = 4
	# 0xf8803038[3:0] = 4
	mask_write 0xf8803038 0x0000000f 0x00000004

	# Config GPIO, PS IO 15->FUNCSEL: GPIO
	# register: FUNCSEL_PS_MIO_15.funcsel_ps_mio_15 = 4
	# 0xf880303c[3:0] = 4
	mask_write 0xf880303c 0x0000000f 0x00000004

	# Config I2C0_scl, PS IO 50->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_50.mc1_ensrc = 0x06
	# 0xf8803994[12:8] = 0x06
	mask_write 0xf8803994 0x00001f00 0x00000600

	# Config v, PS IO 50->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_50.mc1_i_mode = 0x2
	# 0xf8803994[1:0] = 0x2
	mask_write 0xf8803994 0x00000003 0x00000002

	# Config I2C0_sda, PS IO 51->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_51.mc1_ensrc = 0x06
	# 0xf880399c[12:8] = 0x06
	mask_write 0xf880399c 0x00001f00 0x00000600

	# Config v, PS IO 51->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_51.mc1_i_mode = 0x2
	# 0xf880399c[1:0] = 0x2
	mask_write 0xf880399c 0x00000003 0x00000002

	# Config I2C0_scl, PS IO 50->speed: slow
	# register: PARA0_PS_MIO_50.speed = 0x0
	# 0xf8803990[2:0] = 0x0
	mask_write 0xf8803990 0x00000007 0x00000000

	# Config I2C0_scl, PS IO 50->PullType: pullup 25k
	# register: PARA0_PS_MIO_50.PullType = 0x8
	# 0xf8803990[31:28] = 0x8
	mask_write 0xf8803990 0xf0000000 0x80000000

	# Config I2C0_sda, PS IO 51->speed: slow
	# register: PARA0_PS_MIO_51.speed = 0x0
	# 0xf8803998[2:0] = 0x0
	mask_write 0xf8803998 0x00000007 0x00000000

	# Config I2C0_sda, PS IO 51->PullType: pullup 25k
	# register: PARA0_PS_MIO_51.PullType = 0x8
	# 0xf8803998[31:28] = 0x8
	mask_write 0xf8803998 0xf0000000 0x80000000

	# Config I2C0, PS IO 50->FUNCSEL: I2C0
	# register: FUNCSEL_PS_MIO_50.funcsel_ps_mio_50 = 0
	# 0xf88030c8[3:0] = 0
	mask_write 0xf88030c8 0x0000000f 0x00000000

	# Config I2C0, PS IO 51->FUNCSEL: I2C0
	# register: FUNCSEL_PS_MIO_51.funcsel_ps_mio_51 = 0
	# 0xf88030cc[3:0] = 0
	mask_write 0xf88030cc 0x0000000f 0x00000000

	# Config I2C0, Select IO: "PS IO 50...51", emio selection: mio, as a default value
	# register: EMIOSEL_0.emiosel_0 = 1
	# 0xF8803400[0:0] = 1
	mask_write 0xF8803400 0x00000001 0x00000001

	# Config ETH0_mdc, PS IO 52->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_52.mc1_ensrc = 0x06
	# 0xf88039a4[12:8] = 0x06
	mask_write 0xf88039a4 0x00001f00 0x00000600

	# Config v, PS IO 52->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_52.mc1_i_mode = 0x2
	# 0xf88039a4[1:0] = 0x2
	mask_write 0xf88039a4 0x00000003 0x00000002

	# Config ETH0_mdio, PS IO 53->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_53.mc1_ensrc = 0x06
	# 0xf88039ac[12:8] = 0x06
	mask_write 0xf88039ac 0x00001f00 0x00000600

	# Config v, PS IO 53->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_53.mc1_i_mode = 0x2
	# 0xf88039ac[1:0] = 0x2
	mask_write 0xf88039ac 0x00000003 0x00000002

	# Config ETH0_mdc, PS IO 52->speed: medium
	# register: PARA0_PS_MIO_52.speed = 0x2
	# 0xf88039a0[2:0] = 0x2
	mask_write 0xf88039a0 0x00000007 0x00000002

	# Config ETH0_mdc, PS IO 52->PullType: pullup 25k
	# register: PARA0_PS_MIO_52.PullType = 0x8
	# 0xf88039a0[31:28] = 0x8
	mask_write 0xf88039a0 0xf0000000 0x80000000

	# Config ETH0_mdio, PS IO 53->speed: medium
	# register: PARA0_PS_MIO_53.speed = 0x2
	# 0xf88039a8[2:0] = 0x2
	mask_write 0xf88039a8 0x00000007 0x00000002

	# Config ETH0_mdio, PS IO 53->PullType: pullup 25k
	# register: PARA0_PS_MIO_53.PullType = 0x8
	# 0xf88039a8[31:28] = 0x8
	mask_write 0xf88039a8 0xf0000000 0x80000000

	# Config v, PS IO 53->SignalHysteresis: LVCMOS 1.8V
	# register: PARA0_PS_MIO_53.SignalHysteresis = 0x8
	# 0xf88039a8[27:24] = 0x8
	mask_write 0xf88039a8 0x0f000000 0x08000000

	# Config MDIO0, PS IO 52->FUNCSEL: MDIO0
	# register: FUNCSEL_PS_MIO_52.funcsel_ps_mio_52 = 14
	# 0xf88030d0[3:0] = 14
	mask_write 0xf88030d0 0x0000000f 0x0000000e

	# Config MDIO0, PS IO 53->FUNCSEL: MDIO0
	# register: FUNCSEL_PS_MIO_53.funcsel_ps_mio_53 = 14
	# 0xf88030d4[3:0] = 14
	mask_write 0xf88030d4 0x0000000f 0x0000000e

	# Config MDIO0, Select IO: "PS IO 52...53", emio selection: mio, as a default value
	# register: EMIOSEL_13.emiosel_13 = 1
	# 0xF8803434[0:0] = 1
	mask_write 0xF8803434 0x00000001 0x00000001

	# Config PJTAG_tdi, PS IO 10->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_10.mc1_ensrc = 0x06
	# 0xf8803854[12:8] = 0x06
	mask_write 0xf8803854 0x00001f00 0x00000600

	# Config v, PS IO 10->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_10.mc1_i_mode = 0x2
	# 0xf8803854[1:0] = 0x2
	mask_write 0xf8803854 0x00000003 0x00000002

	# Config PJTAG_tdo, PS IO 11->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_11.mc1_ensrc = 0x06
	# 0xf880385c[12:8] = 0x06
	mask_write 0xf880385c 0x00001f00 0x00000600

	# Config v, PS IO 11->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_11.mc1_i_mode = 0x2
	# 0xf880385c[1:0] = 0x2
	mask_write 0xf880385c 0x00000003 0x00000002

	# Config PJTAG_tck, PS IO 12->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_12.mc1_ensrc = 0x06
	# 0xf8803864[12:8] = 0x06
	mask_write 0xf8803864 0x00001f00 0x00000600

	# Config v, PS IO 12->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_12.mc1_i_mode = 0x2
	# 0xf8803864[1:0] = 0x2
	mask_write 0xf8803864 0x00000003 0x00000002

	# Config PJTAG_tms, PS IO 13->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_13.mc1_ensrc = 0x06
	# 0xf880386c[12:8] = 0x06
	mask_write 0xf880386c 0x00001f00 0x00000600

	# Config v, PS IO 13->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_13.mc1_i_mode = 0x2
	# 0xf880386c[1:0] = 0x2
	mask_write 0xf880386c 0x00000003 0x00000002

	# Config PJTAG_tdi, PS IO 10->speed: slow
	# register: PARA0_PS_MIO_10.speed = 0x0
	# 0xf8803850[2:0] = 0x0
	mask_write 0xf8803850 0x00000007 0x00000000

	# Config PJTAG_tdi, PS IO 10->PullType: pullup 25k
	# register: PARA0_PS_MIO_10.PullType = 0x8
	# 0xf8803850[31:28] = 0x8
	mask_write 0xf8803850 0xf0000000 0x80000000

	# Config PJTAG_tdo, PS IO 11->speed: slow
	# register: PARA0_PS_MIO_11.speed = 0x0
	# 0xf8803858[2:0] = 0x0
	mask_write 0xf8803858 0x00000007 0x00000000

	# Config PJTAG_tdo, PS IO 11->PullType: pullup 25k
	# register: PARA0_PS_MIO_11.PullType = 0x8
	# 0xf8803858[31:28] = 0x8
	mask_write 0xf8803858 0xf0000000 0x80000000

	# Config PJTAG_tck, PS IO 12->speed: slow
	# register: PARA0_PS_MIO_12.speed = 0x0
	# 0xf8803860[2:0] = 0x0
	mask_write 0xf8803860 0x00000007 0x00000000

	# Config PJTAG_tck, PS IO 12->PullType: pullup 25k
	# register: PARA0_PS_MIO_12.PullType = 0x8
	# 0xf8803860[31:28] = 0x8
	mask_write 0xf8803860 0xf0000000 0x80000000

	# Config PJTAG_tms, PS IO 13->speed: slow
	# register: PARA0_PS_MIO_13.speed = 0x0
	# 0xf8803868[2:0] = 0x0
	mask_write 0xf8803868 0x00000007 0x00000000

	# Config PJTAG_tms, PS IO 13->PullType: pullup 25k
	# register: PARA0_PS_MIO_13.PullType = 0x8
	# 0xf8803868[31:28] = 0x8
	mask_write 0xf8803868 0xf0000000 0x80000000

	# Config PJTAG, PS IO 10->FUNCSEL: PJTAG
	# register: FUNCSEL_PS_MIO_10.funcsel_ps_mio_10 = 7
	# 0xf8803028[3:0] = 7
	mask_write 0xf8803028 0x0000000f 0x00000007

	# Config PJTAG, PS IO 11->FUNCSEL: PJTAG
	# register: FUNCSEL_PS_MIO_11.funcsel_ps_mio_11 = 7
	# 0xf880302c[3:0] = 7
	mask_write 0xf880302c 0x0000000f 0x00000007

	# Config PJTAG, PS IO 12->FUNCSEL: PJTAG
	# register: FUNCSEL_PS_MIO_12.funcsel_ps_mio_12 = 7
	# 0xf8803030[3:0] = 7
	mask_write 0xf8803030 0x0000000f 0x00000007

	# Config PJTAG, PS IO 13->FUNCSEL: PJTAG
	# register: FUNCSEL_PS_MIO_13.funcsel_ps_mio_13 = 7
	# 0xf8803034[3:0] = 7
	mask_write 0xf8803034 0x0000000f 0x00000007

	# Config qspi_ss0_n, PS IO 1->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_1.mc1_ensrc = 0x06
	# 0xf880380c[12:8] = 0x06
	mask_write 0xf880380c 0x00001f00 0x00000600

	# Config v, PS IO 1->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_1.mc1_i_mode = 0x2
	# 0xf880380c[1:0] = 0x2
	mask_write 0xf880380c 0x00000003 0x00000002

	# Config qspi_clk, PS IO 6->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_6.mc1_ensrc = 0x06
	# 0xf8803834[12:8] = 0x06
	mask_write 0xf8803834 0x00001f00 0x00000600

	# Config v, PS IO 6->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_6.mc1_i_mode = 0x2
	# 0xf8803834[1:0] = 0x2
	mask_write 0xf8803834 0x00000003 0x00000002

	# Config qspi_dq0, PS IO 2->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_2.mc1_ensrc = 0x06
	# 0xf8803814[12:8] = 0x06
	mask_write 0xf8803814 0x00001f00 0x00000600

	# Config v, PS IO 2->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_2.mc1_i_mode = 0x2
	# 0xf8803814[1:0] = 0x2
	mask_write 0xf8803814 0x00000003 0x00000002

	# Config qspi_dq1, PS IO 3->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_3.mc1_ensrc = 0x06
	# 0xf880381c[12:8] = 0x06
	mask_write 0xf880381c 0x00001f00 0x00000600

	# Config v, PS IO 3->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_3.mc1_i_mode = 0x2
	# 0xf880381c[1:0] = 0x2
	mask_write 0xf880381c 0x00000003 0x00000002

	# Config qspi_dq2, PS IO 4->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_4.mc1_ensrc = 0x06
	# 0xf8803824[12:8] = 0x06
	mask_write 0xf8803824 0x00001f00 0x00000600

	# Config v, PS IO 4->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_4.mc1_i_mode = 0x2
	# 0xf8803824[1:0] = 0x2
	mask_write 0xf8803824 0x00000003 0x00000002

	# Config qspi_dq3, PS IO 5->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_5.mc1_ensrc = 0x06
	# 0xf880382c[12:8] = 0x06
	mask_write 0xf880382c 0x00001f00 0x00000600

	# Config v, PS IO 5->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_5.mc1_i_mode = 0x2
	# 0xf880382c[1:0] = 0x2
	mask_write 0xf880382c 0x00000003 0x00000002

	# Config qspi_ss0_n, PS IO 1->speed: fast
	# register: PARA0_PS_MIO_1.speed = 0x7
	# 0xf8803808[2:0] = 0x7
	mask_write 0xf8803808 0x00000007 0x00000007

	# Config qspi_ss0_n, PS IO 1->PullType: pullup 10k
	# register: PARA0_PS_MIO_1.PullType = 0xA
	# 0xf8803808[31:28] = 0xA
	mask_write 0xf8803808 0xf0000000 0xa0000000

	# Config qspi_clk, PS IO 6->speed: fast
	# register: PARA0_PS_MIO_6.speed = 0x7
	# 0xf8803830[2:0] = 0x7
	mask_write 0xf8803830 0x00000007 0x00000007

	# Config qspi_clk, PS IO 6->PullType: pullup 10k
	# register: PARA0_PS_MIO_6.PullType = 0xA
	# 0xf8803830[31:28] = 0xA
	mask_write 0xf8803830 0xf0000000 0xa0000000

	# Config qspi_dq0, PS IO 2->speed: fast
	# register: PARA0_PS_MIO_2.speed = 0x7
	# 0xf8803810[2:0] = 0x7
	mask_write 0xf8803810 0x00000007 0x00000007

	# Config qspi_dq0, PS IO 2->PullType: pullup 10k
	# register: PARA0_PS_MIO_2.PullType = 0xA
	# 0xf8803810[31:28] = 0xA
	mask_write 0xf8803810 0xf0000000 0xa0000000

	# Config qspi_dq1, PS IO 3->speed: fast
	# register: PARA0_PS_MIO_3.speed = 0x7
	# 0xf8803818[2:0] = 0x7
	mask_write 0xf8803818 0x00000007 0x00000007

	# Config qspi_dq1, PS IO 3->PullType: pullup 10k
	# register: PARA0_PS_MIO_3.PullType = 0xA
	# 0xf8803818[31:28] = 0xA
	mask_write 0xf8803818 0xf0000000 0xa0000000

	# Config qspi_dq2, PS IO 4->speed: fast
	# register: PARA0_PS_MIO_4.speed = 0x7
	# 0xf8803820[2:0] = 0x7
	mask_write 0xf8803820 0x00000007 0x00000007

	# Config qspi_dq2, PS IO 4->PullType: pullup 10k
	# register: PARA0_PS_MIO_4.PullType = 0xA
	# 0xf8803820[31:28] = 0xA
	mask_write 0xf8803820 0xf0000000 0xa0000000

	# Config qspi_dq3, PS IO 5->speed: fast
	# register: PARA0_PS_MIO_5.speed = 0x7
	# 0xf8803828[2:0] = 0x7
	mask_write 0xf8803828 0x00000007 0x00000007

	# Config qspi_dq3, PS IO 5->PullType: pullup 10k
	# register: PARA0_PS_MIO_5.PullType = 0xA
	# 0xf8803828[31:28] = 0xA
	mask_write 0xf8803828 0xf0000000 0xa0000000

	# Config qspi_ss1_n_Dot_Enable, PS IO 1->FUNCSEL: Quad_SPI_Flash
	# register: FUNCSEL_PS_MIO_1.funcsel_ps_mio_1 = 0x0
	# 0xf8803004[3:0] = 0x0
	mask_write 0xf8803004 0x0000000f 0x00000000

	# Config qspi_ss1_n_Dot_Enable, PS IO 6->FUNCSEL: Quad_SPI_Flash
	# register: FUNCSEL_PS_MIO_6.funcsel_ps_mio_6 = 0x0
	# 0xf8803018[3:0] = 0x0
	mask_write 0xf8803018 0x0000000f 0x00000000

	# Config qspi_ss1_n_Dot_Enable, PS IO 2->FUNCSEL: Quad_SPI_Flash
	# register: FUNCSEL_PS_MIO_2.funcsel_ps_mio_2 = 0x0
	# 0xf8803008[3:0] = 0x0
	mask_write 0xf8803008 0x0000000f 0x00000000

	# Config qspi_ss1_n_Dot_Enable, PS IO 3->FUNCSEL: Quad_SPI_Flash
	# register: FUNCSEL_PS_MIO_3.funcsel_ps_mio_3 = 0x0
	# 0xf880300c[3:0] = 0x0
	mask_write 0xf880300c 0x0000000f 0x00000000

	# Config qspi_ss1_n_Dot_Enable, PS IO 4->FUNCSEL: Quad_SPI_Flash
	# register: FUNCSEL_PS_MIO_4.funcsel_ps_mio_4 = 0x0
	# 0xf8803010[3:0] = 0x0
	mask_write 0xf8803010 0x0000000f 0x00000000

	# Config qspi_ss1_n_Dot_Enable, PS IO 5->FUNCSEL: Quad_SPI_Flash
	# register: FUNCSEL_PS_MIO_5.funcsel_ps_mio_5 = 0x0
	# 0xf8803014[3:0] = 0x0
	mask_write 0xf8803014 0x0000000f 0x00000000

	# Config SD0_clk, PS IO 40->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_40.mc1_ensrc = 0x06
	# 0xf8803944[12:8] = 0x06
	mask_write 0xf8803944 0x00001f00 0x00000600

	# Config v, PS IO 40->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_40.mc1_i_mode = 0x2
	# 0xf8803944[1:0] = 0x2
	mask_write 0xf8803944 0x00000003 0x00000002

	# Config SD0_cmd, PS IO 41->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_41.mc1_ensrc = 0x06
	# 0xf880394c[12:8] = 0x06
	mask_write 0xf880394c 0x00001f00 0x00000600

	# Config v, PS IO 41->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_41.mc1_i_mode = 0x2
	# 0xf880394c[1:0] = 0x2
	mask_write 0xf880394c 0x00000003 0x00000002

	# Config SD0_d0, PS IO 42->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_42.mc1_ensrc = 0x06
	# 0xf8803954[12:8] = 0x06
	mask_write 0xf8803954 0x00001f00 0x00000600

	# Config v, PS IO 42->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_42.mc1_i_mode = 0x2
	# 0xf8803954[1:0] = 0x2
	mask_write 0xf8803954 0x00000003 0x00000002

	# Config SD0_d1, PS IO 43->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_43.mc1_ensrc = 0x06
	# 0xf880395c[12:8] = 0x06
	mask_write 0xf880395c 0x00001f00 0x00000600

	# Config v, PS IO 43->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_43.mc1_i_mode = 0x2
	# 0xf880395c[1:0] = 0x2
	mask_write 0xf880395c 0x00000003 0x00000002

	# Config SD0_d2, PS IO 44->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_44.mc1_ensrc = 0x06
	# 0xf8803964[12:8] = 0x06
	mask_write 0xf8803964 0x00001f00 0x00000600

	# Config v, PS IO 44->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_44.mc1_i_mode = 0x2
	# 0xf8803964[1:0] = 0x2
	mask_write 0xf8803964 0x00000003 0x00000002

	# Config SD0_d3, PS IO 45->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_45.mc1_ensrc = 0x06
	# 0xf880396c[12:8] = 0x06
	mask_write 0xf880396c 0x00001f00 0x00000600

	# Config v, PS IO 45->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_45.mc1_i_mode = 0x2
	# 0xf880396c[1:0] = 0x2
	mask_write 0xf880396c 0x00000003 0x00000002

	# Config SD0_clk, PS IO 40->speed: fast
	# register: PARA0_PS_MIO_40.speed = 0x7
	# 0xf8803940[2:0] = 0x7
	mask_write 0xf8803940 0x00000007 0x00000007

	# Config SD0_clk, PS IO 40->PullType: pullup 25k
	# register: PARA0_PS_MIO_40.PullType = 0x8
	# 0xf8803940[31:28] = 0x8
	mask_write 0xf8803940 0xf0000000 0x80000000

	# Config SD0_cmd, PS IO 41->speed: fast
	# register: PARA0_PS_MIO_41.speed = 0x7
	# 0xf8803948[2:0] = 0x7
	mask_write 0xf8803948 0x00000007 0x00000007

	# Config SD0_cmd, PS IO 41->PullType: pullup 25k
	# register: PARA0_PS_MIO_41.PullType = 0x8
	# 0xf8803948[31:28] = 0x8
	mask_write 0xf8803948 0xf0000000 0x80000000

	# Config SD0_d0, PS IO 42->speed: fast
	# register: PARA0_PS_MIO_42.speed = 0x7
	# 0xf8803950[2:0] = 0x7
	mask_write 0xf8803950 0x00000007 0x00000007

	# Config SD0_d0, PS IO 42->PullType: pullup 25k
	# register: PARA0_PS_MIO_42.PullType = 0x8
	# 0xf8803950[31:28] = 0x8
	mask_write 0xf8803950 0xf0000000 0x80000000

	# Config SD0_d1, PS IO 43->speed: fast
	# register: PARA0_PS_MIO_43.speed = 0x7
	# 0xf8803958[2:0] = 0x7
	mask_write 0xf8803958 0x00000007 0x00000007

	# Config SD0_d1, PS IO 43->PullType: pullup 25k
	# register: PARA0_PS_MIO_43.PullType = 0x8
	# 0xf8803958[31:28] = 0x8
	mask_write 0xf8803958 0xf0000000 0x80000000

	# Config SD0_d2, PS IO 44->speed: fast
	# register: PARA0_PS_MIO_44.speed = 0x7
	# 0xf8803960[2:0] = 0x7
	mask_write 0xf8803960 0x00000007 0x00000007

	# Config SD0_d2, PS IO 44->PullType: pullup 25k
	# register: PARA0_PS_MIO_44.PullType = 0x8
	# 0xf8803960[31:28] = 0x8
	mask_write 0xf8803960 0xf0000000 0x80000000

	# Config SD0_d3, PS IO 45->speed: fast
	# register: PARA0_PS_MIO_45.speed = 0x7
	# 0xf8803968[2:0] = 0x7
	mask_write 0xf8803968 0x00000007 0x00000007

	# Config SD0_d3, PS IO 45->PullType: pullup 25k
	# register: PARA0_PS_MIO_45.PullType = 0x8
	# 0xf8803968[31:28] = 0x8
	mask_write 0xf8803968 0xf0000000 0x80000000

	# Config SD0, PS IO 40->FUNCSEL: SD0
	# register: FUNCSEL_PS_MIO_40.funcsel_ps_mio_40 = 10
	# 0xf88030a0[3:0] = 10
	mask_write 0xf88030a0 0x0000000f 0x0000000a

	# Config SD0, PS IO 41->FUNCSEL: SD0
	# register: FUNCSEL_PS_MIO_41.funcsel_ps_mio_41 = 10
	# 0xf88030a4[3:0] = 10
	mask_write 0xf88030a4 0x0000000f 0x0000000a

	# Config SD0, PS IO 42->FUNCSEL: SD0
	# register: FUNCSEL_PS_MIO_42.funcsel_ps_mio_42 = 10
	# 0xf88030a8[3:0] = 10
	mask_write 0xf88030a8 0x0000000f 0x0000000a

	# Config SD0, PS IO 43->FUNCSEL: SD0
	# register: FUNCSEL_PS_MIO_43.funcsel_ps_mio_43 = 10
	# 0xf88030ac[3:0] = 10
	mask_write 0xf88030ac 0x0000000f 0x0000000a

	# Config SD0, PS IO 44->FUNCSEL: SD0
	# register: FUNCSEL_PS_MIO_44.funcsel_ps_mio_44 = 10
	# 0xf88030b0[3:0] = 10
	mask_write 0xf88030b0 0x0000000f 0x0000000a

	# Config SD0, PS IO 45->FUNCSEL: SD0
	# register: FUNCSEL_PS_MIO_45.funcsel_ps_mio_45 = 10
	# 0xf88030b4[3:0] = 10
	mask_write 0xf88030b4 0x0000000f 0x0000000a

	# Config SD0, Select IO: "PS IO 40...45", emio selection: mio, as a default value
	# register: EMIOSEL_11.emiosel_11 = 1
	# 0xF880342C[0:0] = 1
	mask_write 0xF880342C 0x00000001 0x00000001

	# Config UART1_tx, PS IO 48->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_48.mc1_ensrc = 0x06
	# 0xf8803984[12:8] = 0x06
	mask_write 0xf8803984 0x00001f00 0x00000600

	# Config v, PS IO 48->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_48.mc1_i_mode = 0x2
	# 0xf8803984[1:0] = 0x2
	mask_write 0xf8803984 0x00000003 0x00000002

	# Config UART1_rx, PS IO 49->Single-ended pull up Drive Strength: 8
	# register: PARA1_PS_MIO_49.mc1_ensrc = 0x06
	# 0xf880398c[12:8] = 0x06
	mask_write 0xf880398c 0x00001f00 0x00000600

	# Config v, PS IO 49->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_49.mc1_i_mode = 0x2
	# 0xf880398c[1:0] = 0x2
	mask_write 0xf880398c 0x00000003 0x00000002

	# Config UART1_tx, PS IO 48->speed: slow
	# register: PARA0_PS_MIO_48.speed = 0x0
	# 0xf8803980[2:0] = 0x0
	mask_write 0xf8803980 0x00000007 0x00000000

	# Config UART1_tx, PS IO 48->PullType: pullup 25k
	# register: PARA0_PS_MIO_48.PullType = 0x8
	# 0xf8803980[31:28] = 0x8
	mask_write 0xf8803980 0xf0000000 0x80000000

	# Config UART1_rx, PS IO 49->speed: slow
	# register: PARA0_PS_MIO_49.speed = 0x0
	# 0xf8803988[2:0] = 0x0
	mask_write 0xf8803988 0x00000007 0x00000000

	# Config UART1_rx, PS IO 49->PullType: pullup 25k
	# register: PARA0_PS_MIO_49.PullType = 0x8
	# 0xf8803988[31:28] = 0x8
	mask_write 0xf8803988 0xf0000000 0x80000000

	# Config UART1, PS IO 48->FUNCSEL: UART1
	# register: FUNCSEL_PS_MIO_48.funcsel_ps_mio_48 = 3
	# 0xf88030c0[3:0] = 3
	mask_write 0xf88030c0 0x0000000f 0x00000003

	# Config UART1, PS IO 49->FUNCSEL: UART1
	# register: FUNCSEL_PS_MIO_49.funcsel_ps_mio_49 = 3
	# 0xf88030c4[3:0] = 3
	mask_write 0xf88030c4 0x0000000f 0x00000003

	# Config UART1, Select IO: "PS IO 48...49", emio selection: mio, as a default value
	# register: EMIOSEL_5.emiosel_5 = 1
	# 0xF8803414[0:0] = 1
	mask_write 0xF8803414 0x00000001 0x00000001

	# Config USB0_stp, PS IO 30->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_30.mc1_ensrc = 0x15
	# 0xf88038f4[12:8] = 0x15
	mask_write 0xf88038f4 0x00001f00 0x00001500

	# Config v, PS IO 30->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_30.mc1_i_mode = 0x2
	# 0xf88038f4[1:0] = 0x2
	mask_write 0xf88038f4 0x00000003 0x00000002

	# Config USB0_dir, PS IO 29->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_29.mc1_ensrc = 0x15
	# 0xf88038ec[12:8] = 0x15
	mask_write 0xf88038ec 0x00001f00 0x00001500

	# Config v, PS IO 29->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_29.mc1_i_mode = 0x2
	# 0xf88038ec[1:0] = 0x2
	mask_write 0xf88038ec 0x00000003 0x00000002

	# Config USB0_nxt, PS IO 31->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_31.mc1_ensrc = 0x15
	# 0xf88038fc[12:8] = 0x15
	mask_write 0xf88038fc 0x00001f00 0x00001500

	# Config v, PS IO 31->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_31.mc1_i_mode = 0x2
	# 0xf88038fc[1:0] = 0x2
	mask_write 0xf88038fc 0x00000003 0x00000002

	# Config USB0_clk, PS IO 36->Single-ended pull up Drive Strength: 16
	# register: PARA1_PS_MIO_36.mc1_ensrc = 0x15
	# 0xf8803924[12:8] = 0x15
	mask_write 0xf8803924 0x00001f00 0x00001500

	# Config v, PS IO 36->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_36.mc1_i_mode = 0x2
	# 0xf8803924[1:0] = 0x2
	mask_write 0xf8803924 0x00000003 0x00000002

	# Config USB0_d7, PS IO 39->Single-ended pull down Drive Strength: 16
	# register: PARA1_PS_MIO_39.mc1_ensnk = 0x17
	# 0xf880393c[20:16] = 0x17
	mask_write 0xf880393c 0x001f0000 0x00170000

	# Config v, PS IO 39->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_39.mc1_i_mode = 0x2
	# 0xf880393c[1:0] = 0x2
	mask_write 0xf880393c 0x00000003 0x00000002

	# Config USB0_d6, PS IO 38->Single-ended pull down Drive Strength: 16
	# register: PARA1_PS_MIO_38.mc1_ensnk = 0x17
	# 0xf8803934[20:16] = 0x17
	mask_write 0xf8803934 0x001f0000 0x00170000

	# Config v, PS IO 38->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_38.mc1_i_mode = 0x2
	# 0xf8803934[1:0] = 0x2
	mask_write 0xf8803934 0x00000003 0x00000002

	# Config USB0_d5, PS IO 37->Single-ended pull down Drive Strength: 16
	# register: PARA1_PS_MIO_37.mc1_ensnk = 0x17
	# 0xf880392c[20:16] = 0x17
	mask_write 0xf880392c 0x001f0000 0x00170000

	# Config v, PS IO 37->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_37.mc1_i_mode = 0x2
	# 0xf880392c[1:0] = 0x2
	mask_write 0xf880392c 0x00000003 0x00000002

	# Config USB0_d4, PS IO 28->Single-ended pull down Drive Strength: 16
	# register: PARA1_PS_MIO_28.mc1_ensnk = 0x17
	# 0xf88038e4[20:16] = 0x17
	mask_write 0xf88038e4 0x001f0000 0x00170000

	# Config v, PS IO 28->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_28.mc1_i_mode = 0x2
	# 0xf88038e4[1:0] = 0x2
	mask_write 0xf88038e4 0x00000003 0x00000002

	# Config USB0_d3, PS IO 35->Single-ended pull down Drive Strength: 16
	# register: PARA1_PS_MIO_35.mc1_ensnk = 0x17
	# 0xf880391c[20:16] = 0x17
	mask_write 0xf880391c 0x001f0000 0x00170000

	# Config v, PS IO 35->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_35.mc1_i_mode = 0x2
	# 0xf880391c[1:0] = 0x2
	mask_write 0xf880391c 0x00000003 0x00000002

	# Config USB0_d2, PS IO 34->Single-ended pull down Drive Strength: 16
	# register: PARA1_PS_MIO_34.mc1_ensnk = 0x17
	# 0xf8803914[20:16] = 0x17
	mask_write 0xf8803914 0x001f0000 0x00170000

	# Config v, PS IO 34->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_34.mc1_i_mode = 0x2
	# 0xf8803914[1:0] = 0x2
	mask_write 0xf8803914 0x00000003 0x00000002

	# Config USB0_d1, PS IO 33->Single-ended pull down Drive Strength: 16
	# register: PARA1_PS_MIO_33.mc1_ensnk = 0x17
	# 0xf880390c[20:16] = 0x17
	mask_write 0xf880390c 0x001f0000 0x00170000

	# Config v, PS IO 33->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_33.mc1_i_mode = 0x2
	# 0xf880390c[1:0] = 0x2
	mask_write 0xf880390c 0x00000003 0x00000002

	# Config USB0_d0, PS IO 32->Single-ended pull down Drive Strength: 16
	# register: PARA1_PS_MIO_32.mc1_ensnk = 0x17
	# 0xf8803904[20:16] = 0x17
	mask_write 0xf8803904 0x001f0000 0x00170000

	# Config v, PS IO 32->mc1_i_mode: LVCMOS 1.8V
	# register: PARA1_PS_MIO_32.mc1_i_mode = 0x2
	# 0xf8803904[1:0] = 0x2
	mask_write 0xf8803904 0x00000003 0x00000002

	# Config USB0_stp, PS IO 30->speed: fast
	# register: PARA0_PS_MIO_30.speed = 0x7
	# 0xf88038f0[2:0] = 0x7
	mask_write 0xf88038f0 0x00000007 0x00000007

	# Config USB0_stp, PS IO 30->PullType: pullup 10k
	# register: PARA0_PS_MIO_30.PullType = 0xA
	# 0xf88038f0[31:28] = 0xA
	mask_write 0xf88038f0 0xf0000000 0xa0000000

	# Config USB0_dir, PS IO 29->speed: fast
	# register: PARA0_PS_MIO_29.speed = 0x7
	# 0xf88038e8[2:0] = 0x7
	mask_write 0xf88038e8 0x00000007 0x00000007

	# Config USB0_dir, PS IO 29->PullType: pullup 10k
	# register: PARA0_PS_MIO_29.PullType = 0xA
	# 0xf88038e8[31:28] = 0xA
	mask_write 0xf88038e8 0xf0000000 0xa0000000

	# Config USB0_nxt, PS IO 31->speed: fast
	# register: PARA0_PS_MIO_31.speed = 0x7
	# 0xf88038f8[2:0] = 0x7
	mask_write 0xf88038f8 0x00000007 0x00000007

	# Config USB0_nxt, PS IO 31->PullType: pullup 10k
	# register: PARA0_PS_MIO_31.PullType = 0xA
	# 0xf88038f8[31:28] = 0xA
	mask_write 0xf88038f8 0xf0000000 0xa0000000

	# Config USB0_clk, PS IO 36->speed: fast
	# register: PARA0_PS_MIO_36.speed = 0x7
	# 0xf8803920[2:0] = 0x7
	mask_write 0xf8803920 0x00000007 0x00000007

	# Config USB0_clk, PS IO 36->PullType: pullup 10k
	# register: PARA0_PS_MIO_36.PullType = 0xA
	# 0xf8803920[31:28] = 0xA
	mask_write 0xf8803920 0xf0000000 0xa0000000

	# Config USB0_d7, PS IO 39->speed: fast
	# register: PARA0_PS_MIO_39.speed = 0x7
	# 0xf8803938[2:0] = 0x7
	mask_write 0xf8803938 0x00000007 0x00000007

	# Config USB0_d7, PS IO 39->PullType: pulldown 10k
	# register: PARA0_PS_MIO_39.PullType = 0x5
	# 0xf8803938[31:28] = 0x5
	mask_write 0xf8803938 0xf0000000 0x50000000

	# Config USB0_d6, PS IO 38->speed: fast
	# register: PARA0_PS_MIO_38.speed = 0x7
	# 0xf8803930[2:0] = 0x7
	mask_write 0xf8803930 0x00000007 0x00000007

	# Config USB0_d6, PS IO 38->PullType: pulldown 10k
	# register: PARA0_PS_MIO_38.PullType = 0x5
	# 0xf8803930[31:28] = 0x5
	mask_write 0xf8803930 0xf0000000 0x50000000

	# Config USB0_d5, PS IO 37->speed: fast
	# register: PARA0_PS_MIO_37.speed = 0x7
	# 0xf8803928[2:0] = 0x7
	mask_write 0xf8803928 0x00000007 0x00000007

	# Config USB0_d5, PS IO 37->PullType: pulldown 10k
	# register: PARA0_PS_MIO_37.PullType = 0x5
	# 0xf8803928[31:28] = 0x5
	mask_write 0xf8803928 0xf0000000 0x50000000

	# Config USB0_d4, PS IO 28->speed: fast
	# register: PARA0_PS_MIO_28.speed = 0x7
	# 0xf88038e0[2:0] = 0x7
	mask_write 0xf88038e0 0x00000007 0x00000007

	# Config USB0_d4, PS IO 28->PullType: pulldown 10k
	# register: PARA0_PS_MIO_28.PullType = 0x5
	# 0xf88038e0[31:28] = 0x5
	mask_write 0xf88038e0 0xf0000000 0x50000000

	# Config USB0_d3, PS IO 35->speed: fast
	# register: PARA0_PS_MIO_35.speed = 0x7
	# 0xf8803918[2:0] = 0x7
	mask_write 0xf8803918 0x00000007 0x00000007

	# Config USB0_d3, PS IO 35->PullType: pulldown 10k
	# register: PARA0_PS_MIO_35.PullType = 0x5
	# 0xf8803918[31:28] = 0x5
	mask_write 0xf8803918 0xf0000000 0x50000000

	# Config USB0_d2, PS IO 34->speed: fast
	# register: PARA0_PS_MIO_34.speed = 0x7
	# 0xf8803910[2:0] = 0x7
	mask_write 0xf8803910 0x00000007 0x00000007

	# Config USB0_d2, PS IO 34->PullType: pulldown 10k
	# register: PARA0_PS_MIO_34.PullType = 0x5
	# 0xf8803910[31:28] = 0x5
	mask_write 0xf8803910 0xf0000000 0x50000000

	# Config USB0_d1, PS IO 33->speed: fast
	# register: PARA0_PS_MIO_33.speed = 0x7
	# 0xf8803908[2:0] = 0x7
	mask_write 0xf8803908 0x00000007 0x00000007

	# Config USB0_d1, PS IO 33->PullType: pulldown 10k
	# register: PARA0_PS_MIO_33.PullType = 0x5
	# 0xf8803908[31:28] = 0x5
	mask_write 0xf8803908 0xf0000000 0x50000000

	# Config USB0_d0, PS IO 32->speed: fast
	# register: PARA0_PS_MIO_32.speed = 0x7
	# 0xf8803900[2:0] = 0x7
	mask_write 0xf8803900 0x00000007 0x00000007

	# Config USB0_d0, PS IO 32->PullType: pulldown 10k
	# register: PARA0_PS_MIO_32.PullType = 0x5
	# 0xf8803900[31:28] = 0x5
	mask_write 0xf8803900 0xf0000000 0x50000000

	# Config USB0, PS IO 30->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_30.funcsel_ps_mio_30 = 15
	# 0xf8803078[3:0] = 15
	mask_write 0xf8803078 0x0000000f 0x0000000f

	# Config USB0, PS IO 29->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_29.funcsel_ps_mio_29 = 15
	# 0xf8803074[3:0] = 15
	mask_write 0xf8803074 0x0000000f 0x0000000f

	# Config USB0, PS IO 31->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_31.funcsel_ps_mio_31 = 15
	# 0xf880307c[3:0] = 15
	mask_write 0xf880307c 0x0000000f 0x0000000f

	# Config USB0, PS IO 36->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_36.funcsel_ps_mio_36 = 15
	# 0xf8803090[3:0] = 15
	mask_write 0xf8803090 0x0000000f 0x0000000f

	# Config USB0, PS IO 39->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_39.funcsel_ps_mio_39 = 15
	# 0xf880309c[3:0] = 15
	mask_write 0xf880309c 0x0000000f 0x0000000f

	# Config USB0, PS IO 38->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_38.funcsel_ps_mio_38 = 15
	# 0xf8803098[3:0] = 15
	mask_write 0xf8803098 0x0000000f 0x0000000f

	# Config USB0, PS IO 37->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_37.funcsel_ps_mio_37 = 15
	# 0xf8803094[3:0] = 15
	mask_write 0xf8803094 0x0000000f 0x0000000f

	# Config USB0, PS IO 28->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_28.funcsel_ps_mio_28 = 15
	# 0xf8803070[3:0] = 15
	mask_write 0xf8803070 0x0000000f 0x0000000f

	# Config USB0, PS IO 35->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_35.funcsel_ps_mio_35 = 15
	# 0xf880308c[3:0] = 15
	mask_write 0xf880308c 0x0000000f 0x0000000f

	# Config USB0, PS IO 34->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_34.funcsel_ps_mio_34 = 15
	# 0xf8803088[3:0] = 15
	mask_write 0xf8803088 0x0000000f 0x0000000f

	# Config USB0, PS IO 33->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_33.funcsel_ps_mio_33 = 15
	# 0xf8803084[3:0] = 15
	mask_write 0xf8803084 0x0000000f 0x0000000f

	# Config USB0, PS IO 32->FUNCSEL: USB0
	# register: FUNCSEL_PS_MIO_32.funcsel_ps_mio_32 = 15
	# 0xf8803080[3:0] = 15
	mask_write 0xf8803080 0x0000000f 0x0000000f

}

# (2) config clock
proc config_clock { } {
	# Config clock source, cpu clock source :  cpu PLL
	# register: CLK_SEL.slow_sel = 1
	# 0xF8801040[4:4] = 1
	mask_write 0xF8801040 0x00000010 0x00000010

	# Config CRG_EN, cpu 4x div1: 4:2:1
	# register: CPU4X_DIV1_PARA.cpu4x_div1_para = 0xffffff
	# 0xF8801010[23:0] = 0xffffff
	mask_write 0xF8801010 0x00ffffff 0x00ffffff

	# Config CRG_EN, cpu 4x div2: 4:2:1
	# register: CPU4X_DIV2_PARA.cpu4x_div2_para = 0x555555
	# 0xF8801014[23:0] = 0x555555
	mask_write 0xF8801014 0x00ffffff 0x00555555

	# Config CRG_EN, cpu 4x div4: 4:2:1
	# register: CPU4X_DIV4_PARA.cpu4x_div4_para = 0x111111
	# 0xF8801018[23:0] = 0x111111
	mask_write 0xF8801018 0x00ffffff 0x00111111

	# Config clock source, cpu clock ratio, 0:421; 1:621
	# register: CLK_SEL.cpu6x4x_sel = 0x0
	# 0xF8801040[0:0] = 0x0
	mask_write 0xF8801040 0x00000001 0x00000000

	# Config clock source, rpu core clock source, 0: cpu2x, 1:cpu4x
	# register: CLK_SEL.rpu4x2x_sel = 0x0
	# 0xF8801040[5:5] = 0x0
	mask_write 0xF8801040 0x00000020 0x00000000

	# Config CPUPLL, cpu pll reset
	# register: CPUPLL_CTRL1.ctrl1_cpupll = 1
	# 0xF8801104[9:9] = 1
	mask_write 0xF8801104 0x00000200 0x00000200

	# Config CPUPLL, CPU N: 96, mc1_fbk_div: 95
	# register: CPUPLL_CTRL9.ctrl9_cpupll = 0x0000005f
	# 0xF8801124[6:0] = 0x0000005f
	mask_write 0xF8801124 0x0000007f 0x0000005f

	# Config CPUPLL, mc1_kvco_sel: 0
	# register: CPUPLL_CTRL9.ctrl9_cpupll = 0x00000000
	# 0xF8801124[13:12] = 0x00000000
	mask_write 0xF8801124 0x00003000 0x00000000

	# Config CPUPLL, mc1_gm_prog: 1
	# register: CPUPLL_CTRL9.ctrl9_cpupll = 0x00000001
	# 0xF8801124[26:24] = 0x00000001
	mask_write 0xF8801124 0x07000000 0x01000000

	# Config CPUPLL, CPU M: 2, mc1_ref_div: 1
	# register: CPUPLL_CTRL8.ctrl8_cpupll = 0x00000001
	# 0xF8801120[6:0] = 0x00000001
	mask_write 0xF8801120 0x0000007f 0x00000001

	# Config CPUPLL, mc1_icp_sel: 10
	# register: CPUPLL_CTRL8.ctrl8_cpupll = 0x0000000a
	# 0xF8801120[20:16] = 0x0000000a
	mask_write 0xF8801120 0x001f0000 0x000a0000

	# Config CPUPLL, mc1_lpf_cap: 2
	# register: CPUPLL_CTRL8.ctrl8_cpupll = 0x00000002
	# 0xF8801120[27:26] = 0x00000002
	mask_write 0xF8801120 0x0c000000 0x08000000

	# Config CPUPLL, mc1_lpf_res: 4
	# register: CPUPLL_CTRL8.ctrl8_cpupll = 0x00000004
	# 0xF8801120[30:28] = 0x00000004
	mask_write 0xF8801120 0x70000000 0x40000000

	# Config CPUPLL, CPU C: 2, mc1_divc1: 1
	# register: CPUPLL_CTRL19.ctrl19_cpupll = 0x00000001
	# 0xF880114C[30:24] = 0x00000001
	mask_write 0xF880114C 0x7f000000 0x01000000

	# Config CPUPLL, CPU C: 2, mc1_duty1: 1
	# register: CPUPLL_CTRL19.ctrl19_cpupll = 0x00000001
	# 0xF880114C[6:0] = 0x00000001
	mask_write 0xF880114C 0x0000007f 0x00000001

	# Config CPUPLL, CPU C: 2, mc1_divc0: 1
	# register: CPUPLL_CTRL18.ctrl18_cpupll = 0x00000001
	# 0xF8801148[30:24] = 0x00000001
	mask_write 0xF8801148 0x7f000000 0x01000000

	# Config CPUPLL, CPU C: 2, mc1_duty0: 1
	# register: CPUPLL_CTRL18.ctrl18_cpupll = 0x00000001
	# 0xF8801148[6:0] = 0x00000001
	mask_write 0xF8801148 0x0000007f 0x00000001

	# Config CPUPLL, cpu pll release
	# register: CPUPLL_CTRL1.ctrl1_cpupll = 0
	# 0xF8801104[9:9] = 0
	mask_write 0xF8801104 0x00000200 0x00000000

	# Config IOPLL, io pll reset
	# register: IOPLL_CTRL1.ctrl1_iopll = 1
	# 0xF8801204[9:9] = 1
	mask_write 0xF8801204 0x00000200 0x00000200

	# Config IOPLL, IO N: 120, mc1_fbk_div: 119
	# register: IOPLL_CTRL9.ctrl9_iopll = 0x00000077
	# 0xF8801224[6:0] = 0x00000077
	mask_write 0xF8801224 0x0000007f 0x00000077

	# Config IOPLL, mc1_kvco_sel: 0
	# register: IOPLL_CTRL9.ctrl9_iopll = 0x00000000
	# 0xF8801224[13:12] = 0x00000000
	mask_write 0xF8801224 0x00003000 0x00000000

	# Config IOPLL, mc1_gm_prog: 1
	# register: IOPLL_CTRL9.ctrl9_iopll = 0x00000001
	# 0xF8801224[26:24] = 0x00000001
	mask_write 0xF8801224 0x07000000 0x01000000

	# Config IOPLL, IO M: 2, mc1_ref_div: 1
	# register: IOPLL_CTRL8.ctrl8_iopll = 0x00000001
	# 0xF8801220[6:0] = 0x00000001
	mask_write 0xF8801220 0x0000007f 0x00000001

	# Config IOPLL, mc1_icp_sel: 10
	# register: IOPLL_CTRL8.ctrl8_iopll = 0x0000000a
	# 0xF8801220[20:16] = 0x0000000a
	mask_write 0xF8801220 0x001f0000 0x000a0000

	# Config IOPLL, mc1_lpf_cap: 2
	# register: IOPLL_CTRL8.ctrl8_iopll = 0x00000002
	# 0xF8801220[27:26] = 0x00000002
	mask_write 0xF8801220 0x0c000000 0x08000000

	# Config IOPLL, mc1_lpf_res: 4
	# register: IOPLL_CTRL8.ctrl8_iopll = 0x00000004
	# 0xF8801220[30:28] = 0x00000004
	mask_write 0xF8801220 0x70000000 0x40000000

	# Config IOPLL, IO1000 C: 2, mc1_divc0: 1
	# register: IOPLL_CTRL18.ctrl18_iopll = 0x00000001
	# 0xF8801248[30:24] = 0x00000001
	mask_write 0xF8801248 0x7f000000 0x01000000

	# Config IOPLL, IO1000 C: 2, mc1_duty0: 1
	# register: IOPLL_CTRL18.ctrl18_iopll = 0x00000001
	# 0xF8801248[6:0] = 0x00000001
	mask_write 0xF8801248 0x0000007f 0x00000001

	# Config IOPLL, IO400 C: 5, mc1_divc1: 4
	# register: IOPLL_CTRL19.ctrl19_iopll = 0x00000004
	# 0xF880124c[30:24] = 0x00000004
	mask_write 0xF880124c 0x7f000000 0x04000000

	# Config IOPLL, IO400 C: 5, mc1_duty1: 3
	# register: IOPLL_CTRL19.ctrl19_iopll = 0x00000003
	# 0xF880124c[6:0] = 0x00000003
	mask_write 0xF880124c 0x0000007f 0x00000003

	# Config IOPLL, IO1000 C: 80, mc1_divc2: 79
	# register: IOPLL_CTRL20.ctrl20_iopll = 0x0000004f
	# 0xF8801250[30:24] = 0x0000004f
	mask_write 0xF8801250 0x7f000000 0x4f000000

	# Config IOPLL, IO1000 C: 80, mc1_duty2: 40
	# register: IOPLL_CTRL20.ctrl20_iopll = 0x00000028
	# 0xF8801250[6:0] = 0x00000028
	mask_write 0xF8801250 0x0000007f 0x00000028

	# Config IOPLL, IO400 C: 25, mc1_divc3: 24
	# register: IOPLL_CTRL21.ctrl21_iopll = 0x00000018
	# 0xF8801254[30:24] = 0x00000018
	mask_write 0xF8801254 0x7f000000 0x18000000

	# Config IOPLL, IO400 C: 25, mc1_duty3: 13
	# register: IOPLL_CTRL21.ctrl21_iopll = 0x0000000d
	# 0xF8801254[6:0] = 0x0000000d
	mask_write 0xF8801254 0x0000007f 0x0000000d

	# Config IOPLL, io pll release
	# register: IOPLL_CTRL1.ctrl1_iopll = 0
	# 0xF8801204[9:9] = 0
	mask_write 0xF8801204 0x00000200 0x00000000

	# Config CPUPLL, CPU PLL WaitLock
	# register: CPUPLL_STATE0.state0_cpupll = 1
	# 0xF8801180[0:0] = 1
	mask_poll 0xF8801180 0x00000001 0x00000001

	# Config IOPLL, IO PLL WaitLock
	# register: IOPLL_STATE0.state0_iopll = 1
	# 0xF8801280[0:0] = 1
	mask_poll 0xF8801280 0x00000001 0x00000001

	# Config clock source, cpu clock source : PLL CLK, pll enable
	# register: CLK_SEL.slow_sel = 0
	# 0xF8801040[4:4] = 0
	mask_write 0xF8801040 0x00000010 0x00000000

	# Config p2f_clk0, system div para for fclk0: 8, based on IO400
	# register: FCLK_CNT_DIV.div_fclk0 = 0x00000007
	# 0xF880103C[5:0] = 0x00000007
	mask_write 0xF880103C 0x0000003f 0x00000007

	# Config QSPI IP Clock, system div para for qspi:10,Clock source IO_1000
	# register: IO1000_CNT_DIV.div_qspi = 0x00000009
	# 0xF8801030[5:0] = 0x00000009
	mask_write 0xF8801030 0x0000003f 0x00000009

	# Config SDIO IP Clock, system div para for sdio:2, clock source: IO_400
	# register: IO1000_CNT_DIV.div_sdio = 0x00000001
	# 0xF8801030[29:24] = 0x00000001
	mask_write 0xF8801030 0x3f000000 0x01000000

	# Config UART IP clock, set uart clock divider. Clock source IO_400
	# register: IO400_DIV8_PARA.io400_div8_para = 0x01010101
	# 0xF8801028[31:0] = 0x01010101
	mask_write 0xF8801028 0xffffffff 0x01010101

}

# (3) config ps_pl
proc config_ps_pl { } {
	# Config PLS_PROT, gp normal acess to pl
	# register: PLS_PROT.gp_proten = 0x0
	# 0xF8800080[1:1] = 0x0
	mask_write 0xF8800080 0x00000002 0x00000000

	# Config PLS_PROT, gp normal acess to pl
	# register: PLS_PROT.gp_proten = 0x0
	# 0xF8800080[1:1] = 0x0
	mask_write 0xF8800080 0x00000002 0x00000000

	# Config CRG_SRST, Slave_AXI_GP0_Interface: reset release
	# register: SRST_CTRL2.nic_gp0m_srst_n = 0x1
	# 0xF8801078[4:4] = 0x1
	mask_write 0xF8801078 0x00000010 0x00000010

	# Config CRG_SRST, Slave_AXI_GP1_Interface: reset release
	# register: SRST_CTRL2.nic_gp1m_srst_n = 0x1
	# 0xF8801078[5:5] = 0x1
	mask_write 0xF8801078 0x00000020 0x00000020

	# Config CRG_SRST, Slave_AXI_HP0_Interface: reset release
	# register: SRST_CTRL2.nic_hp0_srst_n = 0x1
	# 0xF8801078[0:0] = 0x1
	mask_write 0xF8801078 0x00000001 0x00000001

	# Config HP_WIDTH, Slave_AXI_HP0_DATA_WIDTH: 64 bit
	# register: CFG_HP_WIDTH.wmode_hp0 = 0x0
	# 0xF8800084[0:0] = 0x0
	mask_write 0xF8800084 0x00000001 0x00000000

	# Config CRG_SRST, Slave_AXI_HP1_Interface: reset release
	# register: SRST_CTRL2.nic_hp1_srst_n = 0x1
	# 0xF8801078[1:1] = 0x1
	mask_write 0xF8801078 0x00000002 0x00000002

	# Config HP_WIDTH, Slave_AXI_HP1_DATA_WIDTH: 64 bit
	# register: CFG_HP_WIDTH.wmode_hp1 = 0x0
	# 0xF8800084[1:1] = 0x0
	mask_write 0xF8800084 0x00000002 0x00000000

	# Config CRG_SRST, Slave_AXI_ACP_Interface: reset release
	# register: SRST_CTRL0.apu_acp_srst_n = 0x1
	# 0xF8801070[8:8] = 0x1
	mask_write 0xF8801070 0x00000100 0x00000100

	# Config creg, ACP Master is active
	# register: ainacts.ainacts = 0x0
	# 0xF8802004[0:0] = 0x0
	mask_write 0xF8802004 0x00000001 0x00000000

}

# (4) config ddr
proc config_ddr { } {

	# Config DDRC BANK0_GLUE0, Set OE_MD0, 0x2: always off, 0x0:dynamic
	# register: DQSGRP_OE_MD0.oe_md0 = 0x0
	# 0xF842801C[31:0] = 0x0
	mask_write 0xF842801C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE0, Set OE_MD1, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD1.oe_md1 = 0x0
	# 0xF8428020[15:0] = 0x0
	mask_write 0xF8428020 0x0000ffff 0x00000000

	# Config DDRC BANK0_GLUE0, Set TE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_TE_MD0.te_md0 = 0x0
	# 0xF8428024[30:28] = 0x0
	mask_write 0xF8428024 0x70000000 0x00000000

	# Config DDRC BANK0_GLUE0, Set PDR_MD0, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD0.pdr_md0 = 0x0
	# 0xF842802C[31:0] = 0x0
	mask_write 0xF842802C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE0, Set PDR_MD1, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD1.pdr_md1 = 0x0
	# 0xF8428030[15:0] = 0x0
	mask_write 0xF8428030 0x0000ffff 0x00000000

	# Config DDRC BANK0_GLUE1, Set OE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD0.oe_md0 = 0x0
	# 0xF842901C[31:0] = 0x0
	mask_write 0xF842901C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE1, Set OE_MD1, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD1.oe_md1 = 0x0
	# 0xF8429020[15:0] = 0x0
	mask_write 0xF8429020 0x0000ffff 0x00000000

	# Config DDRC BANK0_GLUE1, Set TE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_TE_MD0.te_md0 = 0x0
	# 0xF8429024[30:28] = 0x0
	mask_write 0xF8429024 0x70000000 0x00000000

	# Config DDRC BANK0_GLUE1, Set PDR_MD0, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD0.pdr_md0 = 0x0
	# 0xF842902C[31:0] = 0x0
	mask_write 0xF842902C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE1, Set PDR_MD1, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD1.pdr_md1 = 0x0
	# 0xF8429030[15:0] = 0x0
	mask_write 0xF8429030 0x0000ffff 0x00000000

	# Config DDRC BANK0_GLUE2, Set OE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD0.oe_md0 = 0x0
	# 0xF842a01C[31:0] = 0x0
	mask_write 0xF842a01C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE2, Set OE_MD1, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD1.oe_md1 = 0x0
	# 0xF842a020[15:0] = 0x0
	mask_write 0xF842a020 0x0000ffff 0x00000000

	# Config DDRC BANK0_GLUE2, Set TE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_TE_MD0.te_md0 = 0x0
	# 0xF842a024[30:28] = 0x0
	mask_write 0xF842a024 0x70000000 0x00000000

	# Config DDRC BANK0_GLUE2, Set PDR_MD0, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD0.pdr_md0 = 0x0
	# 0xF842a02C[31:0] = 0x0
	mask_write 0xF842a02C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE2, Set PDR_MD1, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD1.pdr_md1 = 0x0
	# 0xF842a030[15:0] = 0x0
	mask_write 0xF842a030 0x0000ffff 0x00000000

	# Config DDRC BANK0_GLUE3, Set OE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD0.oe_md0 = 0x0
	# 0xF842b01C[31:0] = 0x0
	mask_write 0xF842b01C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE3, Set OE_MD1, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD1.oe_md1 = 0x0
	# 0xF842b020[15:0] = 0x0
	mask_write 0xF842b020 0x0000ffff 0x00000000

	# Config DDRC BANK0_GLUE3, Set TE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_TE_MD0.te_md0 = 0x0
	# 0xF842b024[30:28] = 0x0
	mask_write 0xF842b024 0x70000000 0x00000000

	# Config DDRC BANK0_GLUE3, Set PDR_MD0, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD0.pdr_md0 = 0x0
	# 0xF842b02C[31:0] = 0x0
	mask_write 0xF842b02C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE3, Set PDR_MD1, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD1.pdr_md1 = 0x0
	# 0xF842b030[15:0] = 0x0
	mask_write 0xF842b030 0x0000ffff 0x00000000

	# Config DDRC BANK1_GLUE0, Set OE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD0.oe_md0 = 0x0
	# 0xF842c01C[31:0] = 0x0
	mask_write 0xF842c01C 0xffffffff 0x00000000

	# Config DDRC BANK1_GLUE0, Set OE_MD1, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD1.oe_md1 = 0x0
	# 0xF842c020[15:0] = 0x0
	mask_write 0xF842c020 0x0000ffff 0x00000000

	# Config DDRC BANK1_GLUE0, Set TE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_TE_MD0.te_md0 = 0x0
	# 0xF842c024[30:28] = 0x0
	mask_write 0xF842c024 0x70000000 0x00000000

	# Config DDRC BANK1_GLUE0, Set PDR_MD0, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD0.pdr_md0 = 0x0
	# 0xF842c02C[31:0] = 0x0
	mask_write 0xF842c02C 0xffffffff 0x00000000

	# Config DDRC BANK1_GLUE0, Set PDR_MD1, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD1.pdr_md1 = 0x0
	# 0xF842c030[15:0] = 0x0
	mask_write 0xF842c030 0x0000ffff 0x00000000

	# Config DDRC BANK1_GLUE1, Set OE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD0.oe_md0 = 0x0
	# 0xF842d01C[31:0] = 0x0
	mask_write 0xF842d01C 0xffffffff 0x00000000

	# Config DDRC BANK1_GLUE1, Set OE_MD1, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD1.oe_md1 = 0x0
	# 0xF842d020[15:0] = 0x0
	mask_write 0xF842d020 0x0000ffff 0x00000000

	# Config DDRC BANK1_GLUE1, Set TE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_TE_MD0.te_md0 = 0x0
	# 0xF842d024[30:28] = 0x0
	mask_write 0xF842d024 0x70000000 0x00000000

	# Config DDRC BANK1_GLUE1, Set PDR_MD0, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD0.pdr_md0 = 0x0
	# 0xF842d02C[31:0] = 0x0
	mask_write 0xF842d02C 0xffffffff 0x00000000

	# Config DDRC BANK1_GLUE1, Set PDR_MD1, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD1.pdr_md1 = 0x0
	# 0xF842d030[15:0] = 0x0
	mask_write 0xF842d030 0x0000ffff 0x00000000

	# Config DDRC BANK1_GLUE2, Set OE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD0.oe_md0 = 0x0
	# 0xF842e01C[31:0] = 0x0
	mask_write 0xF842e01C 0xffffffff 0x00000000

	# Config DDRC BANK1_GLUE2, Set OE_MD1, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD1.oe_md1 = 0x0
	# 0xF842e020[15:0] = 0x0
	mask_write 0xF842e020 0x0000ffff 0x00000000

	# Config DDRC BANK1_GLUE2, Set TE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_TE_MD0.te_md0 = 0x0
	# 0xF842e024[30:28] = 0x0
	mask_write 0xF842e024 0x70000000 0x00000000

	# Config DDRC BANK1_GLUE2, Set PDR_MD0, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD0.pdr_md0 = 0x0
	# 0xF842e02C[31:0] = 0x0
	mask_write 0xF842e02C 0xffffffff 0x00000000

	# Config DDRC BANK1_GLUE2, Set PDR_MD1, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD1.pdr_md1 = 0x0
	# 0xF842e030[15:0] = 0x0
	mask_write 0xF842e030 0x0000ffff 0x00000000

	# Config DDRC BANK1_GLUE3, Set OE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD0.oe_md0 = 0x0
	# 0xF842f01C[31:0] = 0x0
	mask_write 0xF842f01C 0xffffffff 0x00000000

	# Config DDRC BANK1_GLUE3, Set OE_MD1, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_OE_MD1.oe_md1 = 0x0
	# 0xF842f020[15:0] = 0x0
	mask_write 0xF842f020 0x0000ffff 0x00000000

	# Config DDRC BANK1_GLUE3, Set TE_MD0, 0x2: always off, 0x0: dynamic
	# register: DQSGRP_TE_MD0.te_md0 = 0x0
	# 0xF842f024[30:28] = 0x0
	mask_write 0xF842f024 0x70000000 0x00000000

	# Config DDRC BANK1_GLUE3, Set PDR_MD0, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD0.pdr_md0 = 0x0
	# 0xF842f02C[31:0] = 0x0
	mask_write 0xF842f02C 0xffffffff 0x00000000

	# Config DDRC BANK0_GLUE3, Set PDR_MD1, 0x1: always on, 0x0: dynamic
	# register: DQSGRP_PDR_MD1.pdr_md1 = 0x0
	# 0xF842f030[15:0] = 0x0
	mask_write 0xF842f030 0x0000ffff 0x00000000


	if {$::DDR_ENABLE == 0 } {
		puts "Info : DDR is disabled"
	}
	if {$::DDR_ENABLE == 1 } {
		# run ddr init elf
		ddr_init
	}
}

# (5) config peripheral
proc config_peripheral { } {
	# Config Quad_SPI_Flash, disable qspi
	# register: SSIENR.SSIC_EN = 0x0
	# 0xF804E008[0:0] = 0x0
	mask_write 0xF804E008 0x00000001 0x00000000

	# Config Quad_SPI_Flash, set SPI Frame Format: SPI_DUAL
	# register: CTRLR0.SPI_FRF = 0x1
	# 0xF804E000[23:22] = 0x1
	mask_write 0xF804E000 0x00c00000 0x00400000

	# Config Quad_SPI_Flash, Transfer Mode RX_ONLY
	# register: CTRLR0.TMOD = 0x2
	# 0xF804E000[11:10] = 0x2
	mask_write 0xF804E000 0x00000c00 0x00000800

	# Config Quad_SPI_Flash, Receive FIFO Threshold 0xFF
	# register: RXFTLR.RFT = 0xFF
	# 0xF804E01C[7:0] = 0xFF
	mask_write 0xF804E01C 0x000000ff 0x000000ff

	# Config Quad_SPI_Flash, enable qspi
	# register: SSIENR.SSIC_EN = 0x1
	# 0xF804E008[0:0] = 0x1
	mask_write 0xF804E008 0x00000001 0x00000001

	# Config UART1, Divisor Latch Access Bit: 1
	# register: LCR.DLAB = 1
	# 0xF840100c[7:7] = 1
	mask_write 0xF840100c 0x00000080 0x00000080

	# Config UART1, Baudrate 115200 bps
	# register: DLH_IER.DLH = 0x00000000
	# 0xF8401004[7:0] = 0x00000000
	mask_write 0xF8401004 0x000000ff 0x00000000

	# Config UART1, Baudrate 115200 bps
	# register: RBR_THR_DLL.DLL = 0x0000001b
	# 0xF8401000[7:0] = 0x0000001b
	mask_write 0xF8401000 0x000000ff 0x0000001b

	# Config UART1, Divisor Latch Access Bit:0
	# register: LCR.DLAB = 0
	# 0xF840100c[7:7] = 0
	mask_write 0xF840100c 0x00000080 0x00000000

	# Config UART1, Data Length Select: 8 data bits per character
	# register: LCR.DLS = 3
	# 0xF840100c[1:0] = 3
	mask_write 0xF840100c 0x00000003 0x00000003

	# Config UART1, Number of stop bits:1 stop bit
	# register: LCR.STOP = 0
	# 0xF840100c[2:2] = 0
	mask_write 0xF840100c 0x00000004 0x00000000

	# Config UART1, FIFO enabled
	# register: IIR_FCR.FIFOE = 1
	# 0xF8401008[0:0] = 1
	mask_write 0xF8401008 0x00000001 0x00000001

}

# (6) config debug
proc config_debug { } {
}

proc platform_init { } {
	halt
	# (1) config psio
	config_psio
	
	# (2) config clock
	config_clock
	
	# (3) config ps_pl
	config_ps_pl

	# (4) config ddr
	config_ddr

	# (5) config peripheral
	config_peripheral

	# (6) config debug
	config_debug

	# (7) config system
	config_system

}

platform_init
