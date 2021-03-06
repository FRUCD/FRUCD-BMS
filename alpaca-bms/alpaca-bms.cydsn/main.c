#include <project.h>
#include <stdint.h>

#include "cell_interface.h"
#include "current_sense.h"
#include "WDT.h"
#include "data.h"
#include "can_manager.h"
#include "uart-terminal.h"
#include "BMS_monitor.h"

//#define WDT_ENABLE

//#define DEBUG_MODE

typedef enum 
{
	BMS_BOOTUP,
	BMS_NORMAL,
	BMS_CHARGEMODE,
	BMS_RACINGMODE,
	BMS_DEBUGMODE,
	BMS_SLEEPMODE,
	BMS_FAULT
}BMS_MODE;


volatile uint8_t CAN_UPDATE_FLAG=0;
extern volatile BAT_PACK_t bat_pack;
extern BAT_SUBPACK_t bat_subpack[N_OF_SUBPACK];
extern volatile BAT_ERR_t* bat_err_array;
extern volatile uint8_t bat_err_index;
extern volatile uint8_t bat_err_index_loop;
volatile uint8_t CAN_DEBUG=0;
volatile uint8_t RACING_FLAG=0;    // this flag should be set in CAN handler
BAT_SOC_t bat_soc;

uint8_t rx_cfg[IC_PER_BUS][8];

void DEBUG_send_cell_voltage();
void DEBUG_send_temp();
void DEBUG_send_current();





CY_ISR(current_update_Handler){
    current_timer_STATUS;
	update_soc();
	return;
}


void process_event(){
    CyGlobalIntDisable
    // heartbeat
    CyDelay(10);
    can_send_status(0xFE,
    	bat_pack.SOC_percent,
    	bat_pack.status,
    	0,0,0);
    CyDelay(10);
    // send voltage   
    can_send_volt(bat_pack.LO_voltage,
				bat_pack.HI_voltage,
				bat_pack.voltage);
    CyDelay(10);

    // send temperature
    
    // TEST_DAY_1
    can_send_temp(bat_pack.subpacks[0]->high_temp,
				bat_pack.subpacks[1]->high_temp,
                bat_pack.subpacks[2]->high_temp,
                bat_pack.subpacks[3]->high_temp,
                bat_pack.subpacks[4]->high_temp,
                bat_pack.subpacks[5]->high_temp,
				bat_pack.HI_temp_node,
				bat_pack.HI_temp_c);
    
    // send current
    //CyDelay(10);
    //can_send_current(bat_pack.current);
    CyDelay(10);
    
    CyGlobalIntEnable;
}

void DEBUG_send_cell_voltage(){
    /*
    uint8_t node, cell;
    for (node = 0; node< N_OF_NODE; node++){
        cell = 0;
        for (cell = 0;cell<14;cell++){
            can_send_volt((node<<8 | cell),
				bat_pack.nodes[node]->cells[cell]->voltage,
				bat_pack.voltage);
            CyDelay(1);
        }
    }
    */
}

void DEBUG_send_temp(){
    /*
    uint8_t node, temp;
    for (node = 0; node< N_OF_NODE; node++){
        temp = 0;
        for (temp = 0;temp<10;temp++){
 //           can_send_temp(temp,
//				node,
//				bat_pack.nodes[node]->temps[temp]->temp_c,
//				bat_pack.nodes[node]->temps[temp]->temp_raw,
  //              bat_pack.HI_temp_c);
            CyDelay(1);
        }
    }
    */
}

void DEBUG_send_current(){
    /*
    uint8_t node, temp;
    for (node = 0; node< N_OF_NODE; node++){
        temp = 0;
        for (temp = 0;temp<10;temp++){
//            can_send_temp(temp,
//				node,
//				bat_pack.nodes[node]->temps[temp]->temp_c,
//				bat_pack.nodes[node]->temps[temp]->temp_raw,
  //              bat_pack.HI_temp_c);
            CyDelay(1);
        }
    }
    */
}


void process_failure_helper(BAT_ERR_t err){
	switch(err.err){
		case CELL_VOLT_OVER:
        	can_send_volt(((err.bad_node<<8) | err.bad_cell),
			    bat_subpack[err.bad_node].cells[err.bad_cell]->voltage, bat_pack.voltage);
		case CELL_VOLT_UNDER:
			can_send_volt(((err.bad_node<<8) | err.bad_cell),
				bat_subpack[err.bad_node].cells[err.bad_cell]->voltage, bat_pack.voltage);
			break;
		case PACK_TEMP_OVER:
		case PACK_TEMP_UNDER:
			// waiting for CAN mesg been defined clearly
			break;

	}
	return;
}

void process_failure(){
	uint8_t i=0;
	// broadcast error in inverse chronological order
	if (bat_err_index_loop){
		// start from bat_err_index back to 0
		for (i=bat_err_index;i>=0;i--){
			process_failure_helper(bat_err_array[i]);
		}
		// start from index=99 to bat_err_index+1
		for (i=99;i>bat_err_index;i--){
			process_failure_helper(bat_err_array[i]);
		}
	}else{
		// start from bat_err_index back to 0
		for (i=bat_err_index;i>=0;i--){
			process_failure_helper(bat_err_array[i]);
		}
	}
}

void debugMain() {
    //while(1) { wakeup_sleep(); };
    uint8_t txData[2] = {0x1E, 0xEE};
    uint8_t rxData[23];
    int t = 0;
    while(1) {

        // For debugging stay in here.
        
        //get_cell_volt();
        //wakeup_sleep();
        CyDelay(500);
        //Select6820_Write(0);
        //my_spi_write_read(txData, 2, rxData, 23);
        //open_wire_adow(0x1);
        //open_wire_adow(0x0);
        
        get_cell_volt();
        get_cell_temp();
        t++;
    }
    while(1) {
        // DO nothing
    }
        
}
bool BALANCE_FLAG = true;
int main(void)
{
    // Stablize the BMS OK signal when system is still setting up
    OK_SIG_Write(1);
    
    
	// Initialize state machine
	BMS_MODE bms_status = BMS_BOOTUP;
	uint32_t system_interval = 0;
    uint8_t led = 0;
   
    
	while(1){
		switch (bms_status){
			case BMS_BOOTUP:
                //current_update_ISR_StartEx(current_update_Handler);
                //current_timer_Start();
				can_init();
				
				#ifdef WDT_ENABLE
                    // TODO Watchdog Timer
			        CyWdtStart(CYWDT_1024_TICKS,CYWDT_LPMODE_NOCHANGE);
                #endif
                
                
				// Initialize
                //SOC_Store_Start();
                //SOC_Timer_Start();
				bms_init();
				mypack_init();
				
                //current_init();
                
                
                
			    //monitor_init();
			    
			    //enable global interrupt
			    CyGlobalIntEnable;
		    
			    //some variables and states
			    OK_SIG_Write(1);
                bms_status = BMS_NORMAL;
		        //terminal_run();
                #ifdef DEBUG_MODE
                    debugMain();
                #endif
				break;

			case BMS_NORMAL:
                // while loop with get volt get temp and bat_balance no delays
                // DCP Enable in 68042.c!!!
			    OK_SIG_Write(1);
			    //check_cfg(rx_cfg);  //CANNOT be finished, because 
				
                
		        get_cell_volt();// TODO test voltage
				//TESTDAY_2_TODO. check_stack_fuse(); // TODO: check if stacks are disconnected
				get_cell_temp();// TODO test temperature
                
                // TODO: Calculate SOC
                //get_current(); // TODO get current reading from sensor
			    //bat_soc = get_soc(); // TODO calculate SOC()
				// because it is normal mode, just set a median length current reading interval
			    
                //SKY_TODO update_soc();

                /*
                //Uncomment all of this to balance
                if (bat_pack.HI_temp_board_c >= 60) {
                    BALANCE_FLAG = false;
                } else if (bat_pack.HI_temp_board_c < 55) {
                    BALANCE_FLAG = true;
                }
                
                
                if (BALANCE_FLAG) {
                    
                    // Turn on cell discharging
                    bat_balance();
                    bat_balance();
                    // Let it discharge for 5 seconds
                    CyDelay(5000);
                    bat_clear_balance();
                    // Let the boards cool down
                    CyDelay(2000);
                } 
                */
                
                bat_health_check();
                if (bat_pack.health == FAULT){
					bms_status = BMS_FAULT;
				}
                
                
                set_current_interval(100);
				system_interval = 500;
				break;
/*
			case BMS_CHARGEMODE:
				OK_SIG_Write(1);

				//check_cfg();  //CANNOT be finished, because 
				//check_cells();// TODO This function will be finished in get_cell_volt/check stack fuse
		        get_cell_volt();// TODO Get voltage
				check_stack_fuse(); // TODO: check if stacks are disconnected
				get_cell_temp();// TODO Get temperature
				get_current(); // TODO get current reading from sensor
				update_soc(); // TODO calculate SOC()
				// because it is normal mode, just set a median length current reading interval
				bat_health_check();
                set_current_interval(100);
				system_interval = 1000;

                if (bat_pack.health == FAULT){
					bms_status = BMS_FAULT;
				}
				if (!(bat_pack.status || CHARGEMODE)){
					bms_status = CHARGEMODE;
				}
				break;

			case BMS_RACINGMODE:
				OK_SIG_Write(1);

				//check_cfg();  //CANNOT be finished, because 
				//check_cells();// TODO This function will be finished in get_cell_volt/check stack fuse
		        get_cell_volt();// TODO Get voltage
				check_stack_fuse(); // TODO: check if stacks are disconnected
				get_cell_temp();// TODO Get temperature
				get_current(); // TODO get current reading from sensor
				bat_soc = get_soc(); // TODO calculate SOC()
                update_soc();
                bat_health_check();
				system_interval = 1000;
				set_current_interval(1);

                
				if (bat_pack.health == FAULT){
					bms_status = BMS_FAULT;
				}
				if (!RACING_FLAG){
					bms_status = BMS_NORMAL;
				}
                
				break;

			case BMS_SLEEPMODE:
				OK_SIG_Write(1);
                bat_health_check();
                if (bat_pack.health == FAULT){
					bms_status = BMS_FAULT;
				}
				break;
*/
			case BMS_FAULT:
				OK_SIG_Write(0u);
				// send
                process_event(); // CAN broadcasting
                /*
                * Sirius: looks like the CAN broadcasting is redundant above. 
                * But I am just being extra cautious for the FE4 competition
                */
				bms_status = BMS_FAULT;
				system_interval = 500;
				process_failure();
                break;
			default:
				bms_status = BMS_FAULT;
                break;
		}
        #ifdef WDT_ENABLE
		    CyWdtClear();
        #endif
		process_event();
		CyDelay(system_interval);
	}
} // main()
