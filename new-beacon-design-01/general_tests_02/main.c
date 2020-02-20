#define NEVER_SLEEP
#define BATTERY_LEVEL_MEAS_INTERVAL          APP_TIMER_TICKS(1, APP_TIMER_PRESCALER)
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_gpio.h"

#include "nrf_delay.h"
#include "nrf51_bitfields.h"

#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
//#include "app_button.h"
#include "ble_nus.h"

#include "ble_error_log.h"
#include "ble_debug_assert_handler.h"

#include "ble_app.h"

#include "SwitchbotNordic.h"
//#include "app_error.h"
//#include "ble.h"
//#include "app_gpiote.h"
#include "nrf_gpiote.h"

//#include "simple_uart.h"



//#define ADVERTISING_LED_PIN_NO          LED_1                                       /**< LED to indicate advertising state. */
//#define CONNECTED_LED_PIN_NO            LED_2                                       /**< LED to indicate connected state. */
#define BUTTON_PULL    					NRF_GPIO_PIN_NOPULL


#define BUTTON_0            			30                           /**< LED to indicate advertising state. */
#define BUTTON_1			            0                                       /**< LED to indicate connected state. */


#define DEVICE_NAME						"SwitchBot"					/* Name of device. Will be included in the advertising data. */


#define APP_ADV_INTERVAL				64						/* The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS		180						/* The advertising timeout (in units of seconds). */

#define APP_TIMER_PRESCALER				0						/* Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS			4						/* Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE			4						/* Size of timer operation queues. */

#define MIN_CONN_INTERVAL				MSEC_TO_UNITS(7.5, UNIT_1_25_MS)		/* Minimum acceptable connection interval (7.5ms). */
#define MAX_CONN_INTERVAL				MSEC_TO_UNITS(40, UNIT_1_25_MS)			/* Maximum acceptable connection interval (40ms). */
#define SLAVE_LATENCY					0						/* Slave latency. */
#define CONN_SUP_TIMEOUT				MSEC_TO_UNITS(300, UNIT_10_MS)			/* Connection supervisory timeout (300ms). */
#define FIRST_CONN_PARAMS_UPDATE_DELAY       32768
//#define FIRST_CONN_PARAMS_UPDATE_DELAY	APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER)	/* Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (1 second). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY       163840
//#define NEXT_CONN_PARAMS_UPDATE_DELAY	APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)	/* Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT	3						/* Number of attempts before giving up the connection parameter negotiation. */

//#define BATTERY_LEVEL_MEAS_INTERVAL		APP_TIMER_TICKS(2000, APP_TIMER_PRESCALER)	/* Battery level measurement interval (ticks). */

#define APP_GPIOTE_MAX_USERS			1						/* Maximum number of users of the GPIOTE handler. */

//#define BUTTON_DETECTION_DELAY			APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)	/* Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define SEC_PARAM_TIMEOUT				30						/* Timeout for Pairing Request or Security Request (in seconds). */
#define SEC_PARAM_BOND					1						/* Perform bonding. */
#define SEC_PARAM_MITM					0						/* Man In The Middle protection not required. */
#define SEC_PARAM_IO_CAPABILITIES		BLE_GAP_IO_CAPS_NONE				/* No I/O capabilities. */
#define SEC_PARAM_OOB					0						/* Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE			7						/* Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE			16						/* Maximum encryption key size. */
#define RESULTS_TABLE_SIZE              80
#define DEAD_BEEF						0xDEADBEEF					/* Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

static ble_gap_sec_params_t 			m_sec_params;			/* Security requirements for this application. */
static uint16_t 						m_conn_handle = BLE_CONN_HANDLE_INVALID;	/* Handle of the current connection. */

static ble_nus_t                        m_nus;  
static ble_app_t 						app;                                    /**< Structure to identify the Nordic UART Service. */
#define SYNC_LENGTH                     2600
uint8_t									r1,r2,r3,r4, r4_old, fr1, fr2, fr3, fr4;
uint8_t									rangeAll,rangeOld;
uint8_t									frangeAll,frangeOld;
uint16_t									intCount,perCount;
uint8_t									fintCount,fperCount;
uint16_t s1,s2,s3,s4;
uint8_t									fs1,fs2,fs3,fs4;
uint32_t								     space = 0, ref_space = 0;
uint8_t                                   data_index = 0;
volatile uint8_t									t1 = 0,t2=0, t3=0, t4=0, t5=0;
uint8_t									IR[8];
uint32_t								debug[15] = {0};
uint32_t								results[RESULTS_TABLE_SIZE][5] = {0};
uint32_t								results_2[17][5] = {0};
uint16_t                                matrix[9][4] = {0};

uint16_t									debug2[200] = {0};
int 									Head, Range, easyRange;

bool 									fNewData = false;
bool 									fDecoy = false;

uint16_t									temp, noDataCount;

char                                    Tir1_string[4];
int16_t									fwd_bwd;
int16_t 								lft_rgt;
uint8_t									command;
uint32_t timer_counter = 0;
static uint8_t                          UART_data_index = 3;

uint32_t timestamp1, timestamp2, max_space, previous_space;
struct qr_cmd cur_cmd;
enum {
	SYNC_WAIT,
	READ_SPACE,
	READ_IR,
	READ_STOP
} m_state = SYNC_WAIT;


bool stop_reading = false;

//static bool charging;									/* true if we're charging */
//
//app_timer_id_t tap_tid;									/* take a pic timer id */
//app_timer_id_t cind_tid;								/* charge indicator timer id */
app_timer_id_t batt_tid;								/* battery timer ID */
//
#define SCHED_MAX_EVENT_DATA_SIZE	sizeof(app_timer_event_t)			/* Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE		10						/* Maximum number of events in the scheduler queue. */

// Persistent storage system event handler
void pstorage_sys_event_handler (uint32_t p_evt);

static void receiver_IRM_coord(void);
void ParseCommandUART(uint8_t cr);







static void LOG2(int input)
{
	char str[24];
	snprintf(str, sizeof str, "%lu", (unsigned long) input);
	LOG(str);
}

static void LOG3(int input)
{
	char str[24];
	snprintf(str, sizeof str, "%d",  input);
	LOG(str);
}

static uint32_t millis(void){
	return timer_counter;
}


void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t *p_file_name)
{
	// This call can be used for debug purposes during application development.
	// @note CAUTION: Activating this code will write the stack to flash on an error.
	//                This function should NOT be used in a final product.
	//                It is intended STRICTLY for development/debugging purposes.
	//                The flash write will happen EVEN if the radio is active, thus interrupting
	//                any communication.
	//                Use with care. Un-comment the line below to use.
	//ble_debug_assert_handler(error_code, line_num, p_file_name);

	// On assert, the system can only recover with a reset.
	//NVIC_SystemReset();
}


void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name)
{
	app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

static void leds_init(void)
{
    nrf_gpio_cfg_output(LED_left);
	nrf_gpio_cfg_output(LED_head);
    nrf_gpio_cfg_output(LED_right);
    nrf_gpio_cfg_output(LED_tail);

    nrf_gpio_cfg_output(RGB_RED);
    nrf_gpio_cfg_output(RGB_GREEN);
    nrf_gpio_cfg_output(RGB_BLUE);

    
    //Turn RGB LEDs ON
 	//nrf_gpio_pin_set(RGB_RED);
	//nrf_gpio_pin_set(RGB_GREEN);
 	nrf_gpio_pin_set(RGB_BLUE);


	//turn all off
	nrf_gpio_pin_set(LED_left);
	nrf_gpio_pin_set(LED_tail);
	nrf_gpio_pin_set(LED_right);
	nrf_gpio_pin_set(LED_head);
	
	//receivers
    nrf_gpio_cfg_input(IRM_head, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(IRM_tail, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(IRM_left, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(IRM_right, NRF_GPIO_PIN_PULLUP);

//    nrf_gpio_cfg_input(IRM_head_far, NRF_GPIO_PIN_PULLUP);
//    nrf_gpio_cfg_input(IRM_tail_far, NRF_GPIO_PIN_PULLUP);
//    nrf_gpio_cfg_input(IRM_left_far, NRF_GPIO_PIN_PULLUP);
//    nrf_gpio_cfg_input(IRM_right_far, NRF_GPIO_PIN_PULLUP);

    nrf_gpio_cfg_input(Switch, NRF_GPIO_PIN_PULLUP);

}



static void uart_init(void)
{
    /**@snippet [UART Initialization] */
    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
    
    NRF_UART0->INTENSET = UART_INTENSET_RXDRDY_Enabled << UART_INTENSET_RXDRDY_Pos;
    
    NVIC_SetPriority(UART0_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(UART0_IRQn);
    /**@snippet [UART Initialization] */
}




/**@brief   Function for handling UART interrupts.
 *
 * @details This function will receive a single character from the UART and append it to a string.
 *          The string will be be sent over BLE when the last character received was a 'new line'
 *          i.e '\n' (hex 0x0D) or if the string has reached a length of @ref NUS_MAX_DATA_LENGTH.
 */
void UART0_IRQHandler(void)
{
    static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    static uint8_t index = 0;
    uint32_t err_code;

    /**@snippet [Handling the data received over UART] */

    data_array[index] = simple_uart_get();
    index++;
	
    /**@snippet [Handling the data received over UART] */
				ParseCommandUART(data_array[index - 1]);
		        index = 0;
		
}




static uint8_t nybble_to_ascii(uint8_t v)
{
	uint8_t ch;

	ch = v & 0x0f;
	if (ch > 9) {
		ch -= 10;
		ch += 'a';
	} else {
		ch += '0';
	}

	return ch;
}

/* prints in decimal with zero suppression */
static void uart_putdec(uint32_t v)
{
	int i, mustprint;

	if (v > 100000) {
		simple_uart_putstring("OVF");
		return;
	}

	mustprint = 0;

	i = v / 100000;
	if (mustprint || i) {
		simple_uart_put(nybble_to_ascii(i));
		v -= i * 100000;
		mustprint = 1;
	}

	i = v / 10000;
	if (mustprint || i) {
		simple_uart_put(nybble_to_ascii(i));
		v -= i * 10000;
		mustprint = 1;
	}

	i = v / 1000;
	if (mustprint || i) {
		simple_uart_put(nybble_to_ascii(i));
		v -= i * 1000;
		mustprint = 1;
	}

	i = v / 100;
	if (mustprint || i) {
		simple_uart_put(nybble_to_ascii(i));
		v -= i * 100;
		mustprint = 1;
	}

	i = v / 10;
	if (mustprint || i) {
		simple_uart_put(nybble_to_ascii(i));
		v -= i * 10;
		mustprint = 1;
	}

	simple_uart_put(nybble_to_ascii(v));
}

static void uart_put32(uint32_t v)
{
	int i;

	simple_uart_put('0');
	simple_uart_put('x');
	for (i=7; i>=0; i--) {
		simple_uart_put(nybble_to_ascii(v >> (i*4)));
	}
}



static void tick_handler(void * p_context)
{
 //   UNUSED_PARAMETER(p_context);
//    battery_start();

  timer_counter++;
}


static void timers_init(void)
{
	// Initialize timer module, making it use the scheduler
	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);


	app_timer_create(&batt_tid, APP_TIMER_MODE_REPEATED, tick_handler);
/*	app_timer_create(&cind_tid, APP_TIMER_MODE_SINGLE_SHOT, charge_indicator_handler);
	app_timer_create(&tap_tid, APP_TIMER_MODE_SINGLE_SHOT, takeapic_handler);
	app_timer_create(&cind_tid, APP_TIMER_MODE_SINGLE_SHOT, charge_indicator_handler);
	app_timer_create(&batt_tid, APP_TIMER_MODE_REPEATED, battery_handler);
*/
}

//=======================================================================
void timer2_init(void) {

	uint32_t crystal_is_running = 0;

	sd_clock_hfclk_request();

	while (!crystal_is_running) {
		sd_clock_hfclk_is_running(&crystal_is_running);
	}
	// Clear TIMER1
	NRF_TIMER1->TASKS_CLEAR = 1;
	NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
	NRF_TIMER1->PRESCALER = 6;  // one ticket is 4us
	NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_16Bit
			<< TIMER_BITMODE_BITMODE_Pos;
	NRF_TIMER1->TASKS_START = 1; // Start clocks

	// Clear TIMER 2
	NRF_TIMER2->TASKS_CLEAR = 1;
	NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
	NRF_TIMER2->PRESCALER = 6;  // one ticket is 4us
	NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit
			<< TIMER_BITMODE_BITMODE_Pos;
	NRF_TIMER2->TASKS_START = 1; // Start clocks

}





static void gap_params_init(void)
{
	uint32_t err_code;
	ble_gap_conn_params_t gap_conn_params;
	ble_gap_conn_sec_mode_t sec_mode;

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

	err_code = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)DEVICE_NAME, strlen(DEVICE_NAME));
	APP_ERROR_CHECK(err_code);

	memset(&gap_conn_params, 0, sizeof(gap_conn_params));

	gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
	gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
	gap_conn_params.slave_latency = SLAVE_LATENCY;
	gap_conn_params.conn_sup_timeout = CONN_SUP_TIMEOUT;

	err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
	APP_ERROR_CHECK(err_code);
}


static void advertising_init(void)
{
	uint32_t err_code;
	ble_advdata_t advdata;
	ble_advdata_t scanrsp;
	uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

	ble_uuid_t adv_uuids[] = {{BLE_UUID_NUS_SERVICE, m_nus.uuid_type}};

	// Build and set advertising data
	memset(&advdata, 0, sizeof(advdata));

	advdata.name_type = BLE_ADVDATA_FULL_NAME;
	advdata.include_appearance = true;  //false in Davin's code
	advdata.flags.size = sizeof(flags);
	advdata.flags.p_data = &flags;

	memset(&scanrsp, 0, sizeof(scanrsp));
	scanrsp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
	scanrsp.uuids_complete.p_uuids  = adv_uuids;

	err_code = ble_advdata_set(&advdata, &scanrsp);
	APP_ERROR_CHECK(err_code);
}



//static void cmd_handler(ble_app_t *app, uint8_t *cmd, uint8_t cmdlen)
//{
/**@brief    Function for handling the data from the Nordic UART Service.
 *
 * @details  This function will process the data received from the Nordic UART BLE Service and send
 *           it to the UART module.
 */
/**@snippet [Handling the data received over BLE] */
void nus_data_handler(ble_nus_t * p_nus, uint8_t * cmd, uint16_t cmdlen)
{

	static uint8_t buf[20];			/* static buffer for data sent back over BLE */
	uint8_t len;				/* how many bytes to return (if any) */
	struct qr_cmd new_cmd;
	uint32_t err_code;

	switch (cmd[0]) {

	case APP_CMD_ARM:

		len = 0;
		break;
	case NOTF_GET_STATUS:
	case NOTF_ACTIVATE_ADB:
	case STAND_UP       :
	case KNEEL          :
	case LEAN           :
	case ESTOP          :
	case CLEAR_ESTOP    :
	        simple_uart_put(cmd[0]);
	        simple_uart_put(cmd[1]);
	        simple_uart_put(0x00);
	        break;

	case DRIVE:
		simple_uart_put(DRIVE);
		simple_uart_put(cmd[1]);
		simple_uart_put(cmd[2]);
//		LOG("DRIVE \n");
		break;



	case 0x06:

		if(cmd[1] == 0x01)
		{

		//eStop
		simple_uart_put(0x65);
		simple_uart_put(0x00);
		simple_uart_put(0x00);


		}

		if(cmd[1] == 0x02)
		{

		//Clear estop
		simple_uart_put(0x66);
		simple_uart_put(0x00);
		simple_uart_put(0x00);
		}


		break;	

	case 'q':  //TIR String

        LOG("[r] %d",IR[0]);
        LOG("[t] %d",IR[1]);
        LOG("[l] %d",IR[2]);
        LOG("[h] %d",IR[3]);

		len = 0;
		break;	

	case 'r':  //TIR String

        LOG("[H] %d",Head);
        LOG("[ER] %d",easyRange);
		len = 0;
		break;



	default:
		len = 0;
		break;
	};

	if (len) {
		//ble_app_tx_blob(app, buf, len);
	}
}




/*
 * toplevel function which initializes all BLE services
 */
static void services_init(void)
{
    uint32_t         err_code;
    ble_nus_init_t   nus_init;
    
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;
    
    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}


/*
 * toplevel function which initializes all BLE services
 */
 /*
static void services_init(void)
{
	ble_app_init_t ai;

//	dis_init();
//	bas_init();

	ai.cmd_handler = cmd_handler;
	ble_app_init(&app, &ai);
}
*/

static void sec_params_init(void)
{
	m_sec_params.timeout = SEC_PARAM_TIMEOUT;
	m_sec_params.bond = SEC_PARAM_BOND;
	m_sec_params.mitm = SEC_PARAM_MITM;
	m_sec_params.io_caps = SEC_PARAM_IO_CAPABILITIES;
	m_sec_params.oob = SEC_PARAM_OOB;
	m_sec_params.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
	m_sec_params.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
}


static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
	if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED) {
		uint32_t err_code;

		err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
		APP_ERROR_CHECK(err_code);
	}
}


static void conn_params_error_handler(uint32_t nrf_error)
{
	APP_ERROR_HANDLER(nrf_error);
}


static void conn_params_init(void)
{
	uint32_t err_code;
	ble_conn_params_init_t cp_init;

	memset(&cp_init, 0, sizeof(cp_init));

	cp_init.p_conn_params = NULL;
	cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
	cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
	cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
	cp_init.start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID;
	cp_init.disconnect_on_fail = false;
	cp_init.evt_handler = on_conn_params_evt;
	cp_init.error_handler = conn_params_error_handler;

	err_code = ble_conn_params_init(&cp_init);
	APP_ERROR_CHECK(err_code);
}


static void timers_start(void)
{
	 uint32_t err_code = app_timer_start(batt_tid, BATTERY_LEVEL_MEAS_INTERVAL, NULL);
	 LOG(" timer start err: %d \n", err_code);
}


static void advertising_start(void)
{
	uint32_t err_code;
	ble_gap_adv_params_t adv_params;

	memset(&adv_params, 0, sizeof(adv_params));

	adv_params.type = BLE_GAP_ADV_TYPE_ADV_IND;
	adv_params.p_peer_addr = NULL;
	adv_params.fp = BLE_GAP_ADV_FP_ANY;
	adv_params.interval = APP_ADV_INTERVAL;
	adv_params.timeout = APP_ADV_TIMEOUT_IN_SECONDS;

	err_code = sd_ble_gap_adv_start(&adv_params);
	APP_ERROR_CHECK(err_code);
}


static void on_ble_evt(ble_evt_t *evt)
{
	uint32_t err_code;
	static ble_gap_evt_auth_status_t m_auth_status;
	ble_gap_enc_info_t *enc_info;

	switch (evt->header.evt_id) {
	case BLE_GAP_EVT_CONNECTED:
	        //nrf_gpio_pin_set(CONNECTED_LED_PIN_NO);
            //nrf_gpio_pin_clear(LED_1);
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		    //nrf_gpio_pin_clear(CONNECTED_LED_PIN_NO);
			advertising_start();
		break;

	case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
		err_code = sd_ble_gap_sec_params_reply(app.conn_handle, BLE_GAP_SEC_STATUS_SUCCESS, &m_sec_params);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GATTS_EVT_SYS_ATTR_MISSING:
		err_code = sd_ble_gatts_sys_attr_set(app.conn_handle, NULL, 0);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GAP_EVT_AUTH_STATUS:
		m_auth_status = evt->evt.gap_evt.params.auth_status;
		break;

	case BLE_GAP_EVT_SEC_INFO_REQUEST:
		enc_info = &m_auth_status.periph_keys.enc_info;
		if (enc_info->div == evt->evt.gap_evt.params.sec_info_request.div) {
			err_code = sd_ble_gap_sec_info_reply(app.conn_handle, enc_info, NULL);
			APP_ERROR_CHECK(err_code);
		} else {
			// No keys found for this device
			err_code = sd_ble_gap_sec_info_reply(app.conn_handle, NULL, NULL);
			APP_ERROR_CHECK(err_code);
		}
		break;

	case BLE_GAP_EVT_TIMEOUT:
		if (evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISEMENT) {
#ifdef NEVER_SLEEP
			advertising_init();
			advertising_start();
#else
			if (charging) {
				advertising_init();
				advertising_start();
			} else {
				/* make sure the LEDs are off */
				//nrf_gpio_pin_clear(LED_1);
				//nrf_gpio_pin_clear(LED_2);

				// Configure buttons with sense level low as wakeup source.
				nrf_gpio_cfg_sense_input(BUTTON_0, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
				nrf_gpio_cfg_sense_input(BUTTON_1, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);

				// Go to system-off mode (this function will not return; wakeup will cause a reset)
				err_code = sd_power_system_off();
				APP_ERROR_CHECK(err_code);
			}
#endif
		}
		break;

	case BLE_EVT_TX_COMPLETE:
		break;

	default:
		// No implementation needed.
		break;
	};
}


/*static void ble_evt_dispatch(ble_evt_t *evt)
{
	on_ble_evt(evt);
	ble_conn_params_on_ble_evt(evt);
	ble_app_on_ble_evt(&app, evt);
}*/

static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    ble_nus_on_ble_evt(&m_nus, p_ble_evt);
    on_ble_evt(p_ble_evt);
}	


static void sys_evt_dispatch(uint32_t sys_evt)
{
	pstorage_sys_event_handler(sys_evt);
}


static void ble_stack_init(void)
{
	uint32_t err_code;
	// Initialize the SoftDevice handler module.
	SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_4000MS_CALIBRATION, false);

	// Register with the SoftDevice handler module for BLE events.
	err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
	APP_ERROR_CHECK(err_code);

	// Register with the SoftDevice handler module for BLE events.
	err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
	APP_ERROR_CHECK(err_code);
}


static void scheduler_init(void)
{
	//APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}




static void gpiote_init(void)
{
	//APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
}




static void power_manage(void)
{
	uint32_t err_code = sd_app_evt_wait();
	APP_ERROR_CHECK(err_code);
}



static void receiver_IRM_coord(void)
{

    r1  = ( nrf_gpio_pin_read(IRM_right) == 0 );
	r2  = ( nrf_gpio_pin_read(IRM_tail)  == 0 );
	r3  = ( nrf_gpio_pin_read(IRM_left)  == 0 );
	r4  = ( nrf_gpio_pin_read(IRM_head)  == 0 );		//Invert and check each IR receivers in turn and store its state.


 	debug[8]++;

	rangeAll = r1 | r2 | r3 | r4;
	intCount++;														//increment the ramp counter

	if (r1) s1++;  //For each receiver, if its high, increment the counter
	if (r2) s2++;  //this block shoulfd probably be in the else of the IF below)
	if (r3) s3++;
	if (r4) s4++;




	if ( (rangeOld == 1) && (rangeAll == 0) )							//look for falling edge
	{																
		perCount = intCount;														//set the counter
		intCount = 0;																

//    	debug[0] = s1;
//    	debug[1] = s2;
//    	debug[2] = s3;
//    	debug[3] = s4;
    	results[debug[6]][0] = s1;
    	results[debug[6]][1] = s2;
    	results[debug[6]][2] = s3;
    	results[debug[6]][3] = s4;
    	results[debug[6]][4] = debug[8];
//		debug[4] = s4 + s2 + s3 + s1;
//		debug[5] = perCount;
		debug[6]++;
//		debug[9]  = debug[7]; // we save the old value
		//debug[7]  = space;
 //       debug[10] = debug[8];

		s1 = 0; s2 = 0; s3 = 0; s4 = 0;									//reset the counters
      //   space = 0;
		//if(perCount > 395)
		 fNewData = true;
		if(debug[6] == RESULTS_TABLE_SIZE -1) debug[6] =0;

	}							//look for falling edge



	rangeOld = rangeAll;
}





void ParseCommandUART(uint8_t cr)
{   
    uint32_t err_code;
  
                //If waiting for a new command
                if (UART_data_index == 3)
                {
                    //Figure out which tyoe of command it is:
                    switch (cr)
                    {
                    	case 0x61: //standup
                    	case 0x62: //kneel
                    	case 0x63: // lean (one parameter)
                    	case 0x65: //estop
                    	case 0x66: //clear estop
                        case 'x'        :
                        case NOTF_SET_STATUS:
                        case NOTF_GET_STATUS:

                        				command = cr;                       
                                        UART_data_index = 0;   
                                  
                                        /*
                                        err_code = ble_nus_send_string(&m_nus, "X", 1);
                                        if (err_code != NRF_ERROR_INVALID_STATE)
                                        {
                                                APP_ERROR_CHECK(err_code);
                                        }
                                        */
                        
                                        break;

                                       
                        default         :                           //do nothinh
                                                                          
                                        //err_code = ble_nus_send_string(&m_nus, "?", 1);
                                        if (err_code != NRF_ERROR_INVALID_STATE)
                                        {
                                                APP_ERROR_CHECK(err_code);
                                        }
                                        break;

                    }

                }   else { //its an ongoing command

                                                
                    //Ready for the data
                    if (UART_data_index == 0) //its the fwd_bwd byte
                    {
                         fwd_bwd = cr;
                         UART_data_index = 1;
                      //  simple_uart_put('F');
                       // simple_uart_put(UART_data_index);
                    } else {
                            if (UART_data_index == 1) //its the lft_rgt byte
                            {
                                lft_rgt = cr;
                                LOG("----\n");
                                
                                //all data received, time to calculate and drive
                                UART_data_index = 3;
                              if(command == NOTF_GET_STATUS){
                        	//Send data over bluetooth
                                 uint8_t data[3] = {command, fwd_bwd, lft_rgt};
                                 //err_code = ble_nus_send_string(&m_nus, data, 3);
                                 LOG(" counter index %d \n ", debug[6]);
                                 for(uint8_t i=0; i < RESULTS_TABLE_SIZE ; i++){
                                	LOG("  %6d, %6d, %6d, %6d, %6d   %6d \n", results[i][0], results[i][1], results[i][2], results[i][3], results[i][4], i);

                                 }

                              }else{
                                //Send data over the uart
                                simple_uart_put(command);
                                simple_uart_put(fwd_bwd);
                                simple_uart_put(lft_rgt);
                              }

                        }
                    }
                            
                    
                }

}



uint8_t perCount_copy;
uint8_t ir_values[8];
uint8_t tmp_tab1[4];
uint8_t angle, angle_far;
int tmp_head, tmp_head_far;


void calcul_01(void){

	uint8_t data_out[4];
		if(fNewData)
			{
//			nrf_gpio_pin_set(TX_MEDIA_BOX);
			debug[0]++;
			uint32_t err_code;
			  LOG("--------   %6d  %6d \n", debug[0], debug[1]);

			for (uint8_t i = 0; i < 12; i++) {
				LOG(" %6d %6d %6d %6d  %6d  \n", results_2[i][0],
						results_2[i][1], results_2[i][2],
						results_2[i][3], results_2[i][4]);

			}

	           memset(&results_2[0][0], 0, sizeof(results_2));
	           fNewData = false;
	            nrf_delay_ms(20);
//			nrf_gpio_pin_clear(TX_MEDIA_BOX);
			} else {
				debug[1]++;
			}


}



void calcul_02(void) {

	uint8_t data_out[4];
	if (fNewData) {
		debug[0]++;
		uint32_t err_code;
		LOG("--------   %6d  %6d \n\n\n", debug[0], debug[1]);

		// ignoring bad readings
		bool is_good_reading = false;
		if ((results_2[0][4] > 26000) && (results_2[0][4] < 27000)) {
			for (uint8_t i = 1; i < 12; i++) {
				if ((results_2[i][4] > 26000) && (results_2[i][4] < 27000)) {
					is_good_reading = true;
				}
			}
			fNewData = true;
		}

		if (is_good_reading) {

			for (uint8_t i = 0; i < 13; i++) {
				LOG(" %4d   %4d  %4d  %4d %5d \n", results_2[i][0],
						results_2[i][1], results_2[i][2], results_2[i][3],
						results_2[i][4]);
			}
			LOG("***----- \n");

			uint8_t data[10] = { 0 };
			data[0] = 1;
			uint8_t i = 1;
			bool is_transmission_completed = false;
			while ((i < 13) && !is_transmission_completed) {
				data[1] = (uint8_t)((results_2[i][4] - 1000) / 2514); // calculating the row index in the reading matrix

				if (data[1] < 9) {

					//filter_data(&results_2[i], data[1]);
					uint8_t k = 2;
					for (uint8_t j = 0; j < 4; j++) {
						data[k] = (uint8_t) results_2[i][j];
						k++;
						data[k] = (results_2[i][j] >> 8);
						k++;
					}

					LOG(" %6d %6d %6d %6d %6d %6d \n", data[1], results_2[i][0],
							results_2[i][1], results_2[i][2], results_2[i][3], results_2[i][4]);
					uint32_t err_code = ble_nus_send_string(&m_nus, data, 10);

				}

				nrf_delay_ms(20);

				data[0] = 0;
				if (results_2[i + 1][4] > 26000) {
					is_transmission_completed = true;
				}
				i++;
			}

		}

		memset(&results_2[0][0], 0, sizeof(results_2));
		fNewData = false;
	} else {
		debug[1]++;
	}

}


void calcul_03(void){

	uint8_t data_out[4];
	if (fNewData) {
		debug[0]++;
		uint32_t err_code;
		LOG("--------   %6d  %6d \n", debug[0], debug[1]);

		// ignoring bad readings
		bool is_good_reading = false;
		if ((results_2[0][4] > 21000) && (results_2[0][4] < 27000)) {
			for (uint8_t i = 1; i < 12; i++) {
				if ((results_2[i][4] > 21000) && (results_2[i][4] < 27000)) {
					is_good_reading = true;
				}
			}
			fNewData = true;
		}

		if (is_good_reading) {

			bool is_transmission_completed = false;

		for(uint8_t i=0; i< 10; i++){
			uint8_t index = (uint8_t)((((results_2[i][4] * 4) - 4000) / 8000) - 1);

            LOG(" %6d %6d %6d %6d %6d \n", index,
 			    		  results_2[i][0], results_2[i][1] ,
			    		  results_2[i][2], results_2[i][3]
            );

			//     uint32_t      err_code = ble_nus_send_string(&m_nus, data, 9);
              if(results_2[ i + 1][4] > 2000){
            	  is_transmission_completed = true;
              }
			}
		}

		memset(&results_2[0][0], 0, sizeof(results_2));
		fNewData = false;
		nrf_delay_ms(10);
	} else {
		debug[1]++;
	}

}


void calcul_04(void){

	uint8_t data_out[4];
		if (fNewData) {
			debug[0]++;
			uint32_t err_code;
			LOG("--------   %6d  %6d \n", debug[0], debug[1]);

			// ignoring bad readings
			bool is_good_reading = true;


			if (is_good_reading) {
	            uint8_t data[10] = {0};
	            data[0] = 1;
//					 data[1] = (uint8_t)((results_2[i][4] - 1000 ) / 2514);

                      for(uint8_t i = 0; i < 10; i++)
			           LOG(" %6d %6d %6d %6d %6d  \n",
			 			    		  results_2[i][0], results_2[i][1] ,
						    		  results_2[i][2], results_2[i][3], results_2[i][4]
			            );



				 //    uint32_t      err_code = ble_nus_send_string(&m_nus, data, 10);

	         		nrf_delay_ms(120);



			}

			memset(&results_2[0][0], 0, sizeof(results_2));
			fNewData = false;
		} else {
			debug[1]++;
		}
}

void calcul_05(void) {

	uint8_t data_out[4];
	if (fNewData) {
		debug[0]++;
		uint32_t err_code;
		LOG("--------   %6d  %6d \n\n\n", debug[0], debug[1]);

		// ignoring bad readings
		bool is_good_reading = false;
		if ((results_2[0][4] > 26000) && (results_2[0][4] < 27000)) {
			for (uint8_t i = 1; i < 12; i++) {
				if ((results_2[i][4] > 26000) && (results_2[i][4] < 27000)) {
					is_good_reading = true;
				}
			}
			fNewData = true;
		}

		if (is_good_reading) {

			for (uint8_t i = 0; i < 13; i++) {
				LOG(" %4d   %4d  %4d  %4d %5d \n", results_2[i][0],
						results_2[i][1], results_2[i][2], results_2[i][3],
						results_2[i][4]);
			}

			LOG("***----- \n");

            uint16_t     reading_matrix[9][4] = {0};
			uint8_t i = 1;
			bool is_transmission_completed = false;
			while ((i < 13) && !is_transmission_completed) {
				uint8_t index  = (uint8_t)((results_2[i][4] - 1000) / 2514); // calculating the row index in the reading matrix

				if (index < 9) {
					for (uint8_t j = 0; j < 4; j++) {
						reading_matrix[index][j] = (uint16_t) results_2[i][j];
					}
				}

				if (results_2[i + 1][4] > 26000) {
					is_transmission_completed = true;
				}
				i++;
			}

			// filtering
			filter_data_2d(reading_matrix);

			// sending data
			uint8_t data[10] = { 0 };
			data[0] = 1;
			for (uint8_t i2 = 0; i2 < 9; i2++) {
				uint8_t k = 2;
				data[1] = i2;
				for (uint8_t j = 0; j < 4; j++) {
					data[k] = (uint8_t) reading_matrix[i2][j];
					k++;
					data[k] = (reading_matrix[i2][j] >> 8);
					k++;
				}
				uint32_t err_code = ble_nus_send_string(&m_nus, data, 10);
				nrf_delay_ms(20);
				data[0] = 0;
			}
		}

		memset(&results_2[0][0], 0, sizeof(results_2));
		fNewData = false;
	} else {
		debug[1]++;
	}

}


void calcul_06(void) {

	uint8_t data_out[4];
	if (fNewData) {

//		for(uint8_t i = 0; i < 9; i++){
//     	     LOG(" %4d %4d %4d %4d \n", matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3]);
//		}
//
//		LOG("--------------- \n");
//        nrf_delay_ms(60);
//	    memset(&matrix, 0, sizeof(matrix));
		fNewData = false;
	}
}




uint8_t get_tx_id(uint32_t space){

	if( (space > 1500) && (space < 2500)) return 0;
	if( (space > 8500) && (space < 9500)) return 1;
	if( (space > 15500) && (space < 17500)) return 2;
	if( (space > 22500) && (space < 24000)) return 3;
	if( (space > 29500) && (space < 31000)) return 4;
	if( (space > 36500) && (space < 38000)) return 5;
	if( (space > 44000) && (space < 46000)) return 6;
}



bool is_sync(uint32_t space){
return	(debug[7] > 3000) && (debug[7] < 4500);
}




void receive_ppi_enable_channel(uint32_t ch_num, volatile uint32_t *event_ptr, volatile uint32_t *task_ptr)
{
   uint32_t err_code;
   uint32_t error;
       err_code =  sd_ppi_channel_assign(ch_num, event_ptr, task_ptr);
       LOG(" ppi assign error : %d \n" , err_code);
     if( err_code != NRF_SUCCESS ){
                    error = err_code;//erreur
                }
      err_code =  sd_ppi_channel_enable_set(1 << ch_num);
       LOG(" ppi enable error : %d \n" , err_code);
                if( err_code != NRF_SUCCESS ){
                    error = err_code;//erreur
                }
}



void receive_gpiote_init(){


     nrf_gpio_cfg_sense_input(IRM_head, GPIO_PIN_CNF_PULL_Pullup, NRF_GPIO_PIN_SENSE_LOW);
     nrf_gpio_cfg_sense_input(IRM_left, GPIO_PIN_CNF_PULL_Pullup, NRF_GPIO_PIN_SENSE_LOW);
     nrf_gpio_cfg_sense_input(IRM_right, GPIO_PIN_CNF_PULL_Pullup, NRF_GPIO_PIN_SENSE_LOW);
     nrf_gpio_cfg_sense_input(IRM_tail, GPIO_PIN_CNF_PULL_Pullup, NRF_GPIO_PIN_SENSE_LOW);

     NRF_GPIOTE->INTENSET = (GPIOTE_INTENSET_IN0_Enabled << GPIOTE_INTENSET_IN0_Pos);
     NRF_GPIOTE->INTENSET = (GPIOTE_INTENSET_IN1_Enabled << GPIOTE_INTENSET_IN1_Pos);
     NRF_GPIOTE->INTENSET = (GPIOTE_INTENSET_IN2_Enabled << GPIOTE_INTENSET_IN2_Pos);
     NRF_GPIOTE->INTENSET = (GPIOTE_INTENSET_IN3_Enabled << GPIOTE_INTENSET_IN3_Pos);
//
     nrf_gpiote_event_config(0, IRM_right,  NRF_GPIOTE_POLARITY_TOGGLE);
     nrf_gpiote_event_config(1, IRM_tail,  NRF_GPIOTE_POLARITY_TOGGLE);
     nrf_gpiote_event_config(2, IRM_left, NRF_GPIOTE_POLARITY_TOGGLE);
     nrf_gpiote_event_config(3, IRM_head,  NRF_GPIOTE_POLARITY_TOGGLE);

//     sd_nvic_SetPriority(GPIOTE_IRQn, 3);
     NVIC_SetPriority(GPIOTE_IRQn, 3);
//     sd_nvic_EnableIRQ(GPIOTE_IRQn);
     NVIC_EnableIRQ(GPIOTE_IRQn);



        receive_ppi_enable_channel(0,&(NRF_GPIOTE->EVENTS_IN[0]),&(NRF_TIMER1->TASKS_CAPTURE[0]));
        receive_ppi_enable_channel(1,&(NRF_GPIOTE->EVENTS_IN[1]),&(NRF_TIMER1->TASKS_CAPTURE[1]));
        receive_ppi_enable_channel(2,&(NRF_GPIOTE->EVENTS_IN[2]),&(NRF_TIMER1->TASKS_CAPTURE[2]));
        receive_ppi_enable_channel(3,&(NRF_GPIOTE->EVENTS_IN[3]),&(NRF_TIMER1->TASKS_CAPTURE[3]));



}


void GPIOTE_IRQHandler_copy2(void) {

//    nrf_gpio_pin_set(TX_MEDIA_BOX);
	r1 = (nrf_gpio_pin_read(IRM_right) == 0);
	r2 = (nrf_gpio_pin_read(IRM_tail) == 0);
	r3 = (nrf_gpio_pin_read(IRM_left) == 0);
	r4 = (nrf_gpio_pin_read(IRM_head) == 0); //Invert and check each IR receivers in turn and store its state.

	rangeAll = r1 | r2 | r3 | r4;

	if (NRF_GPIOTE->EVENTS_IN[0] == 1) {
	    results[debug[6]][0] = NRF_TIMER1->CC[0];
	    NRF_TIMER1->TASKS_CLEAR = 1;
	    debug[6]++;
	    NRF_GPIOTE->EVENTS_IN[0] = 0;

	    if(debug[6] >= (RESULTS_TABLE_SIZE -1)){
	      debug[6] = 0;
	    }
	}



rangeOld = rangeAll;


 //   nrf_gpio_pin_clear(TX_MEDIA_BOX);

}




void GPIOTE_IRQHandler_copy3(void) {

//    nrf_gpio_pin_set(TX_MEDIA_BOX);

	r4 = (nrf_gpio_pin_read(IRM_head) == 0); //Invert and check each IR receivers in turn and store its state.
	r3 = (nrf_gpio_pin_read(IRM_left) == 0); //Invert and check each IR receivers in turn and store its state.

	rangeAll = r3 | r4;


	if (NRF_GPIOTE->EVENTS_IN[3] == 1) {
		if (r4 == 0) {
			results[debug[6]][3] = NRF_TIMER1->CC[3] - results[debug[6]][3];
			NRF_TIMER1->CC[3] = 0;
		} else {
			results[debug[6]][3] = NRF_TIMER1->CC[3];
		}
		NRF_GPIOTE->EVENTS_IN[3] = 0;
	}

	if (NRF_GPIOTE->EVENTS_IN[2] == 1) {
	  if (r3 == 0) {
			results[debug[6]][2] = NRF_TIMER1->CC[2] - results[debug[6]][2];
			NRF_TIMER1->CC[2] = 0;
		} else {
			results[debug[6]][2] = NRF_TIMER1->CC[2];
		}


		NRF_GPIOTE->EVENTS_IN[2] = 0;
	}

	if ((rangeOld == 1) && (rangeAll == 0)) { // detect the falling edge
		NRF_TIMER2->TASKS_CAPTURE[0] = 1;
		results[debug[6]][4] = NRF_TIMER2->CC[0];
		//	NRF_TIMER2->CC[0] = 0;
//		NRF_TIMER2->TASKS_CLEAR = 1;
		if ((results[debug[6]][0] > 2000)
				|| (results[debug[6]][1] > 2000)  // detect the sync burst
				|| (results[debug[6]][2] > 2000)
				|| (results[debug[6]][3] > 2000)) {

			NRF_TIMER2->TASKS_CLEAR = 1;
			//debug[6] = 0;

			if ((debug[6] > 11) && (!fNewData)) {
				uint8_t index = debug[6], index2 = 0;
				bool copy_completed = false;

				results_2[index2][0] = results[index][0];
				results_2[index2][1] = results[index][1];
				results_2[index2][2] = results[index][2];
				results_2[index2][3] = results[index][3];
				results_2[index2][4] = results[index][4];

				index--;
				index2++;
				while (!copy_completed && (index2 < 13)) {
					results_2[index2][0] = results[index][0];
					results_2[index2][1] = results[index][1];
					results_2[index2][2] = results[index][2];
					results_2[index2][3] = results[index][3];
					results_2[index2][4] = results[index][4];
					if ((results[index][0] > 2600) || (results[index][1] > 2600)
							|| (results[index][2] > 2600)
							|| (results[index][3] > 2600)) {
						copy_completed = true;
					}
					index--;
					index2++;
				}
				index2--;

				fNewData = true;

			}
		}

		debug[6]++;
		NRF_TIMER1->TASKS_CLEAR = 1;

		if (debug[6] == RESULTS_TABLE_SIZE - 1) {
			debug[6] = 0;
		}
	    // preparing the new line
		results[debug[6]][0] = 0;
		results[debug[6]][1] = 0;
		results[debug[6]][2] = 0;
		results[debug[6]][3] = 0;
		results[debug[6]][4] = 0;


	}

	rangeOld = rangeAll;


//    nrf_gpio_pin_clear(TX_MEDIA_BOX);

}


void GPIOTE_IRQHandler(void) {

//    nrf_gpio_pin_set(TX_MEDIA_BOX);
	r1 = (nrf_gpio_pin_read(IRM_right) == 0);
	r2 = (nrf_gpio_pin_read(IRM_tail) == 0);
	r3 = (nrf_gpio_pin_read(IRM_left) == 0);
	r4 = (nrf_gpio_pin_read(IRM_head) == 0); //Invert and check each IR receivers in turn and store its state.

	rangeAll = r1 | r2 | r3 | r4;

	if (NRF_GPIOTE->EVENTS_IN[0] == 1) {
		if (r1 == 0) {
			results[debug[6]][0] = NRF_TIMER1->CC[0] - results[debug[6]][0];
			NRF_TIMER1->CC[0] = 0;

		} else {
			results[debug[6]][0] = NRF_TIMER1->CC[0];

		}
		NRF_GPIOTE->EVENTS_IN[0] = 0;
	} else {

		//	results[debug[6]][0] = 0;
	}

	if (NRF_GPIOTE->EVENTS_IN[1] == 1) {
		if (r2 == 0) {
			results[debug[6]][1] = NRF_TIMER1->CC[1] - results[debug[6]][1];
			NRF_TIMER1->CC[1] = 0;
		} else {
			results[debug[6]][1] = NRF_TIMER1->CC[1];
		}
		NRF_GPIOTE->EVENTS_IN[1] = 0;
	} else {

	//	results[debug[6]][1] = 0;
	}

	if (NRF_GPIOTE->EVENTS_IN[2] == 1) {
		if (r3 == 0) {
			results[debug[6]][2] = NRF_TIMER1->CC[2] - results[debug[6]][2];
			NRF_TIMER1->CC[2] = 0;
		} else {
			results[debug[6]][2] = NRF_TIMER1->CC[2];
		}
		NRF_GPIOTE->EVENTS_IN[2] = 0;
	} else {

		//	results[debug[6]][2] = 0;
	}

	if (NRF_GPIOTE->EVENTS_IN[3] == 1) {
		if (r4 == 0) {
			results[debug[6]][3] = NRF_TIMER1->CC[3] - results[debug[6]][3];
			NRF_TIMER1->CC[3] = 0;
		} else {
			results[debug[6]][3] = NRF_TIMER1->CC[3];
		}
		NRF_GPIOTE->EVENTS_IN[3] = 0;
	} else {

		//	results[debug[6]][3] = 0;
	}

	if ((rangeOld == 1) && (rangeAll == 0)) { // detect the falling edge

		NRF_TIMER2->TASKS_CAPTURE[0] = 1;
		results[debug[6]][4] = NRF_TIMER2->CC[0];
		//	NRF_TIMER2->CC[0] = 0;
//		NRF_TIMER2->TASKS_CLEAR = 1;
		if ((results[debug[6]][0] > SYNC_LENGTH)
				|| (results[debug[6]][1] > SYNC_LENGTH)  // detect the sync burst
				|| (results[debug[6]][2] > SYNC_LENGTH)
				|| (results[debug[6]][3] > SYNC_LENGTH)) {

			NRF_TIMER2->TASKS_CLEAR = 1;
			//debug[6] = 0;

			if ((debug[6] > 11) && (!fNewData)) {
				uint8_t index = debug[6], index2 = 0;
				bool copy_completed = false;

				results_2[index2][0] = results[index][0];
				results_2[index2][1] = results[index][1];
				results_2[index2][2] = results[index][2];
				results_2[index2][3] = results[index][3];
				results_2[index2][4] = results[index][4];

				index--;
				index2++;
				while (!copy_completed && (index2 < 13)) {
					results_2[index2][0] = results[index][0];
					results_2[index2][1] = results[index][1];
					results_2[index2][2] = results[index][2];
					results_2[index2][3] = results[index][3];
					results_2[index2][4] = results[index][4];
					if ((results[index][0] > SYNC_LENGTH) || (results[index][1] > SYNC_LENGTH)
							|| (results[index][2] > SYNC_LENGTH)
							|| (results[index][3] > SYNC_LENGTH)) {
						copy_completed = true;
					}
					index--;
					index2++;
				}

				fNewData = true;

			}
		}

		debug[6]++;
		NRF_TIMER1->TASKS_CLEAR = 1;

		if (debug[6] == RESULTS_TABLE_SIZE - 1) {
			debug[6] = 0;
		}

		results[debug[6]][0] = 0;
		results[debug[6]][1] = 0;
		results[debug[6]][2] = 0;
		results[debug[6]][3] = 0;
		results[debug[6]][4] = 0;

	}

	rangeOld = rangeAll;


//    nrf_gpio_pin_clear(TX_MEDIA_BOX);

}


void GPIOTE_IRQHandler_copy1(void) {

//    nrf_gpio_pin_set(TX_MEDIA_BOX);
	r1 = (nrf_gpio_pin_read(IRM_right) == 0);
	r2 = (nrf_gpio_pin_read(IRM_tail) == 0);
	r3 = (nrf_gpio_pin_read(IRM_left) == 0);
	r4 = (nrf_gpio_pin_read(IRM_head) == 0); //Invert and check each IR receivers in turn and store its state.

	rangeAll = r1 | r2 | r3 | r4;

	if (NRF_GPIOTE->EVENTS_IN[0] == 1) {
		if (r1 == 0) {
			results[debug[6]][0] = NRF_TIMER1->CC[0] - results[debug[6]][0];
			NRF_TIMER1->CC[0] = 0;

		} else {
			results[debug[6]][0] = NRF_TIMER1->CC[0];

		}
		NRF_GPIOTE->EVENTS_IN[0] = 0;
	}

	if (NRF_GPIOTE->EVENTS_IN[1] == 1) {
		if (r2 == 0) {
			results[debug[6]][1] = NRF_TIMER1->CC[1] - results[debug[6]][1];
			NRF_TIMER1->CC[1] = 0;
		} else {
			results[debug[6]][1] = NRF_TIMER1->CC[1];
		}
		NRF_GPIOTE->EVENTS_IN[1] = 0;
	}

	if (NRF_GPIOTE->EVENTS_IN[2] == 1) {
		if (r3 == 0) {
			results[debug[6]][2] = NRF_TIMER1->CC[2] - results[debug[6]][2];
			NRF_TIMER1->CC[2] = 0;
		} else {
			results[debug[6]][2] = NRF_TIMER1->CC[2];
		}
		NRF_GPIOTE->EVENTS_IN[2] = 0;
	}

	if (NRF_GPIOTE->EVENTS_IN[3] == 1) {
		if (r4 == 0) {
			results[debug[6]][3] = NRF_TIMER1->CC[3] - results[debug[6]][3];
			NRF_TIMER1->CC[3] = 0;
		} else {
			results[debug[6]][3] = NRF_TIMER1->CC[3];
		}
		NRF_GPIOTE->EVENTS_IN[3] = 0;
	}


	if ((rangeOld == 1) && (rangeAll == 0)) { // detect the falling edge

		NRF_TIMER2->TASKS_CAPTURE[0] = 1;
		results[debug[6]][4] = NRF_TIMER2->CC[0];

		if (!fNewData) {

				uint8_t index  = (uint8_t)((results[debug[6]][4] - 1000) / 2514); // calculating the row index in the reading matrix
                if(index < 9){
                    matrix[index][0] = results[debug[6]][0];
                    matrix[index][1] = results[debug[6]][1];
                    matrix[index][2] = results[debug[6]][2];
                    matrix[index][3] = results[debug[6]][3];

                }
			}

		if ((results[debug[6]][0] > SYNC_LENGTH)
				|| (results[debug[6]][1] > SYNC_LENGTH)  // detect the sync burst
				|| (results[debug[6]][2] > SYNC_LENGTH)
				|| (results[debug[6]][3] > SYNC_LENGTH)) {

			NRF_TIMER2->TASKS_CLEAR = 1;

			if((results[debug[6]][4] > 26000) && (results[debug[6]][4] < 27000)){
              fNewData = true;
		    }else{
	          memset(&matrix, 0, sizeof(matrix));
		    }

		}

		debug[6]++;
		NRF_TIMER1->TASKS_CLEAR = 1;

		if (debug[6] == RESULTS_TABLE_SIZE - 1) {
			debug[6] = 0;
		}

		results[debug[6]][0] = 0;
		results[debug[6]][1] = 0;
		results[debug[6]][2] = 0;
		results[debug[6]][3] = 0;
		results[debug[6]][4] = 0;

	}

	rangeOld = rangeAll;


}



int main(void)
{
	uart_init();


    nrf_gpio_cfg_output(TX_MEDIA_BOX);
    nrf_gpio_pin_clear(TX_MEDIA_BOX);
	leds_init();
	timers_init();

	gpiote_init();
	ble_stack_init();
	scheduler_init();
	gap_params_init();
	services_init();
	advertising_init();
	conn_params_init();
	sec_params_init();

	timers_start();

//	app_button_enable();
	advertising_start();
	timer2_init();
	receive_gpiote_init();




    LOG("app started ...  %3d %3d \n",sizeof(results_2),  NEXT_CONN_PARAMS_UPDATE_DELAY);


    timestamp1 = debug[8];
    max_space = 0;

   // activating the motors - clr-estop
    nrf_delay_ms(100);
	simple_uart_put(0x66);
	simple_uart_put(0x00);
	simple_uart_put(0x00);

	while (1) {

       calcul_02();
	   power_manage();


	};
}





