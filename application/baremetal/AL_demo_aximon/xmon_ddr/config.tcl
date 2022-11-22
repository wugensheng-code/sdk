proc rd { Addr } {
set address [format "0x%08X" $Addr]
set len 1
create_hw_axi_txn read_txn [get_hw_axis hw_axi_i] -type read -address $address -len $len
run_hw_axi [get_hw_axi_txns read_txn]
set read_proc [list [report_hw_axi_txn [get_hw_axi_txns read_txn]]];
set read_status_value [lindex $read_proc 0 1]
delete_hw_axi_txn [get_hw_axi_txns read_txn]
return $read_status_value;
}
proc wr { Addr data} {
set len 1
set address [format "0x%08X" $Addr]
set data [format "0x%08X" $data]
create_hw_axi_txn write_txn [get_hw_axis hw_axi_i] -type write -address $address -len $len -data $data
run_hw_axi [get_hw_axi_txns write_txn]
delete_hw_axi_txn [get_hw_axi_txns write_txn]
}