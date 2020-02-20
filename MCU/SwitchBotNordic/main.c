#define NEVER_SLEEP

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
#include "app_button.h"
#include "ble_nus.h"

#include "ble_error_log.h"
#include "ble_debug_assert_handler.h"

#include "ble_app.h"

#include "SwitchbotNordic.h"
//#include "app_error.h"
//#include "ble.h"
#include "app_gpiote.h"

#include "simple_uart.h"



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
#define FIRST_CONN_PARAMS_UPDATE_DELAY	APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER)	/* Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (1 second). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY	APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)	/* Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT	3						/* Number of attempts before giving up the connection parameter negotiation. */

#define BATTERY_LEVEL_MEAS_INTERVAL		APP_TIMER_TICKS(2000, APP_TIMER_PRESCALER)	/* Battery level measurement interval (ticks). */

#define APP_GPIOTE_MAX_USERS			1						/* Maximum number of users of the GPIOTE handler. */

#define BUTTON_DETECTION_DELAY			APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)	/* Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define SEC_PARAM_TIMEOUT				30						/* Timeout for Pairing Request or Security Request (in seconds). */
#define SEC_PARAM_BOND					1						/* Perform bonding. */
#define SEC_PARAM_MITM					0						/* Man In The Middle protection not required. */
#define SEC_PARAM_IO_CAPABILITIES		BLE_GAP_IO_CAPS_NONE				/* No I/O capabilities. */
#define SEC_PARAM_OOB					0						/* Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE			7						/* Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE			16						/* Maximum encryption key size. */

#define DEAD_BEEF						0xDEADBEEF					/* Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

static ble_gap_sec_params_t 			m_sec_params;			/* Security requirements for this application. */
static uint16_t 						m_conn_handle = BLE_CONN_HANDLE_INVALID;	/* Handle of the current connection. */

static ble_nus_t                        m_nus;  
static ble_app_t 						app;                                    /**< Structure to identify the Nordic UART Service. */

uint8_t									r1,r2,r3,r4;
uint8_t									rangeAll,rangeOld;
uint8_t									intCount,perCount;
uint8_t									s1,s2,s3,s4;
uint8_t									IR[4];
int 									Head, Range, easyRange;

bool 									fNewData = false;
bool 									fDecoy = false;

uint16_t									temp, noDataCount;

char                                    Tir1_string[4];
int16_t									fwd_bwd;
int16_t 								lft_rgt;
uint8_t									command;


static uint8_t                          UART_data_index = 3;


struct qr_cmd cur_cmd;

//static bool charging;									/* true if we're charging */

app_timer_id_t tap_tid;									/* take a pic timer id */
app_timer_id_t cind_tid;								/* charge indicator timer id */
app_timer_id_t batt_tid;								/* battery timer ID */

#define SCHED_MAX_EVENT_DATA_SIZE	sizeof(app_timer_event_t)			/* Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE		10						/* Maximum number of events in the scheduler queue. */

// Persistent storage system event handler
void pstorage_sys_event_handler (uint32_t p_evt);

static void receiver_IRM_coord(void);
void ParseCommandUART(uint8_t cr);

int _write(int file, char *ptr, int len) {
    uint32_t err_code = ble_nus_send_string(&m_nus, ptr, len);
    APP_ERROR_CHECK(err_code);
    return len;
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
 	nrf_gpio_pin_set(RGB_RED);
	nrf_gpio_pin_set(RGB_GREEN);
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



static void battery_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    battery_start();
}


static void timers_init(void)
{
	// Initialize timer module, making it use the scheduler
	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, true);

/*
	app_timer_create(&tap_tid, APP_TIMER_MODE_SINGLE_SHOT, takeapic_handler);
	app_timer_create(&cind_tid, APP_TIMER_MODE_SINGLE_SHOT, charge_indicator_handler);
	app_timer_create(&batt_tid, APP_TIMER_MODE_REPEATED, battery_handler);
*/
}

//=======================================================================
void timer2_init(void)
{
	//Initialize timer1.
	NRF_TIMER2->INTENCLR          = 0xffffffffUL;
	NRF_TIMER2->TASKS_STOP        = 1;
	NRF_TIMER2->TASKS_CLEAR       = 1;
	NRF_TIMER2->MODE              = TIMER_MODE_MODE_Timer;
	NRF_TIMER2->EVENTS_COMPARE[0] = 0;
	NRF_TIMER2->EVENTS_COMPARE[1] = 0;
	NRF_TIMER2->EVENTS_COMPARE[2] = 0;
	NRF_TIMER2->EVENTS_COMPARE[3] = 0;
	NRF_TIMER2->SHORTS            = 0;
	NRF_TIMER2->PRESCALER         = 3;																// Input clock is 16MHz, timer clock = 2^3.
	NRF_TIMER2->BITMODE           = TIMER_BITMODE_BITMODE_32Bit;
	NRF_TIMER2->INTENSET          = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);
	NRF_TIMER2->SHORTS            = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
	NRF_TIMER2->CC[0]             = 200;
	NRF_TIMER2->TASKS_START       = 1;

	NVIC_EnableIRQ(TIMER2_IRQn);
}


//=======================================================================
//=======================================================================
void TIMER2_IRQHandler(void)
{


	//nrf_gpio_pin_toggle(LED_4);

	receiver_IRM_coord();				//calculate the relative distance and the angle coord.

	//nrf_gpio_pin_toggle(LED_4);

	//nrf_gpio_pin_toggle(LED_1);

	NRF_TIMER2->EVENTS_COMPARE[0] = 0;
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

	case 'x':
		simple_uart_put('x');
		simple_uart_put(cmd[1]);
		simple_uart_put(cmd[2]);
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

        printf("[r] %d",IR[0]);
        printf("[t] %d",IR[1]);
        printf("[l] %d",IR[2]);
        printf("[h] %d",IR[3]);
										
		len = 0;
		break;	

	case 'r':  //TIR String

        printf("[H] %d",Head);
        printf("[ER] %d",easyRange);	
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
	APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}




static void gpiote_init(void)
{
	APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
}




static void power_manage(void)
{
	uint32_t err_code = sd_app_evt_wait();
	APP_ERROR_CHECK(err_code);
}



static void receiver_IRM_coord(void)
{
	r1 = ( nrf_gpio_pin_read(IRM_right) == 0 );
	r2 = ( nrf_gpio_pin_read(IRM_tail)  == 0 );
	r3 = ( nrf_gpio_pin_read(IRM_left)  == 0 );
	r4 = ( nrf_gpio_pin_read(IRM_head)  == 0 );		//Invert and check each IR receivers in turn and store its state.


	rangeAll = r1 | r2 | r3 | r4;									//Variable to hold "collective state"
	intCount++;														//increment the ramp counter

	if (r1) s1++;  //For each receiver, if its high, increment the counter
	if (r2) s2++;  //this block shoulfd probably be in the else of the IF below)
	if (r3) s3++;
	if (r4) s4++;							

	if ( (rangeOld == 1) && (rangeAll == 0) )							//look for falling edge
	{																
		perCount = intCount;														//set the counter
		intCount = 0;																

		IR[0] = s1; IR[1] = s2; IR[2] = s3; IR[3] = s4;				//Save the counters from the previous ramps
		s1 = 0; s2 = 0; s3 = 0; s4 = 0;									//reset the counters

		fNewData = true;
	}

	rangeOld = rangeAll;
}

int calc_coord(void)
{

int sVal;
int head, theta;
int mIdx, i;
int mVal, minV;
int IRc[4];

// find max value in the array [M2 M3 M4 M1]

//   M3 - Back
//   M0 - Left
//   M1 - Front
//   M2 - Right


   mIdx = 0;
   mVal = IR[0];
   minV = IR[0];

    // find which receiver get the maximum intensity
    for (i = 1; i < 4; i++)
    {
       if (IR[i] > mVal)
       {
         mIdx = i;
         mVal = IR[i];
       }

       if (IR[i] < minV)
       {
         minV = IR[i];
       }
    }


    for (i = 0; i < 4; i++)
    {
         IRc[i] = IR[i] - minV;
    }



    // use the max intensity to orient the RX array
    // calculate the angle as a weighteed average
   switch (mIdx)
   {
      case 0:           //max = R0 - Right
         sVal = (IRc[3] + IRc[1] + IRc[0]);
         mVal = ((IR[3] + IR[1])>>1) + IR[0];
         if (sVal == 0) sVal = 1;
         head = (IRc[1] - IRc[3]);
         theta = (head * 90) / sVal;
         theta = theta - 90;

			nrf_gpio_pin_clear(LED_tail);
        	nrf_gpio_pin_clear(LED_right);
   			nrf_gpio_pin_set(LED_left);
        	nrf_gpio_pin_set(LED_head);


         break;

      case 1:           // max = R1 - Back
         sVal = (IRc[2] + IRc[0] + IRc[1]);
         mVal = ((IR[2] + IR[0])>>1) + IR[1];
         if (sVal == 0) sVal = 1;
         head = (IRc[2] - IRc[0]);
         theta = (head * 90) / sVal;

        	nrf_gpio_pin_clear(LED_left);
			nrf_gpio_pin_clear(LED_tail);
   			nrf_gpio_pin_set(LED_right);
        	nrf_gpio_pin_set(LED_head);

         break;

      case 2:           // max = R2 - Left
         sVal = (IRc[3] + IRc[1] + IRc[2]);
         mVal = ((IR[3] + IR[1])>>1) + IR[2];
         if (sVal == 0) sVal = 1;
         head = (IRc[3] - IRc[1]);
         theta = (head * 90) / sVal;
         theta = theta + 90;

        	nrf_gpio_pin_clear(LED_left);
			nrf_gpio_pin_clear(LED_head);
   			nrf_gpio_pin_set(LED_right);
        	nrf_gpio_pin_set(LED_tail);


         break;

      case 3:           // max = R4 - Front
         sVal = (IRc[2] + IRc[0] + IRc[3]);
         mVal = ((IR[2] + IR[0])>>1) + IR[3];
         if (sVal == 0) sVal = 1;
         head = (IRc[0] - IRc[2]);
         theta = (head * 90) / sVal;
         theta = theta + 180;
         if (theta > 180)
         {
            theta = theta - 360;
         }

        	nrf_gpio_pin_clear(LED_right);
			nrf_gpio_pin_clear(LED_head);
   			nrf_gpio_pin_set(LED_left);
        	nrf_gpio_pin_set(LED_tail);

         break;
   }

    Head = theta;
    Range = mVal;
    easyRange = 125 - Range;

    return 1;
}

void motionControl_YAW(void)
{
   	uint16_t yawSpeed;

    yawSpeed = 180 - abs(Head);

    yawSpeed = yawSpeed / 4;

    if(yawSpeed > 0)
    {
    	yawSpeed += 2;
    }

    if(yawSpeed > 16)
    {
    	yawSpeed = 16;
    }


    if (Head > 0)  //turn left
    {
        lft_rgt = 0x60 + yawSpeed;
    }
    else
    {
        lft_rgt = 0x40 + yawSpeed;
    }

}


void motionControl_DriveFWD(void)
{
   	uint16_t fwdSpeed;
   	uint16_t distance;

   	distance = 30;

    fwdSpeed = abs(easyRange - distance);

    if(fwdSpeed > 0)
    {

		fwdSpeed += 2;    	
    }

    if(fwdSpeed > 16)
    {
    	fwdSpeed = 16;
    }

    if ((180 - abs(Head)) <= 30)  //pitch forward
    {

	    if(easyRange > distance) //move forward
	    {
	        fwd_bwd = 0x00 + fwdSpeed;
	    	        			

	    } else {

		    fwd_bwd = 0x00;

	    }

    } else {
        
        	fwd_bwd = 0x00;
    }

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
                                                                          
                                        err_code = ble_nus_send_string(&m_nus, "?", 1);
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
                        //simple_uart_put('F');
                    } else {
                            if (UART_data_index == 1) //its the lft_rgt byte
                            {
                                lft_rgt = cr;
                                
                                //all data received, time to calculate and drive
                                UART_data_index = 3;

                                //Send data over the uart
                                simple_uart_put(command);
                                simple_uart_put(fwd_bwd);
                                simple_uart_put(lft_rgt);

                            }
                    }
                            
                    
                   
                    
                }                
}






int main(void)
{
	uart_init();
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

	timer2_init();
	timers_start();


	app_button_enable();
	advertising_start();


	//turn all LEDs on
	nrf_gpio_pin_clear(LED_left);
	nrf_gpio_pin_clear(LED_tail);
	nrf_gpio_pin_clear(LED_right);
	nrf_gpio_pin_clear(LED_head);

	nrf_delay_ms(100);

	//turn all LEDs off
	nrf_gpio_pin_set(LED_left);
	nrf_gpio_pin_set(LED_tail);
	nrf_gpio_pin_set(LED_right);
	nrf_gpio_pin_set(LED_head);

	nrf_delay_ms(100);

	//turn all LEDs on
	nrf_gpio_pin_clear(LED_left);
	nrf_gpio_pin_clear(LED_tail);
	nrf_gpio_pin_clear(LED_right);
	nrf_gpio_pin_clear(LED_head);

	nrf_delay_ms(100);

	//turn all LEDs off
	nrf_gpio_pin_set(LED_left);
	nrf_gpio_pin_set(LED_tail);
	nrf_gpio_pin_set(LED_right);
	nrf_gpio_pin_set(LED_head);

	nrf_delay_ms(100);





	while (1) {
		nrf_delay_ms(50);
		app_sched_execute();


		if(fNewData)
		{
			fNewData = false;

			calc_coord();
			motionControl_YAW();
			motionControl_DriveFWD();

			simple_uart_put(0x78);
			simple_uart_put(fwd_bwd);
			simple_uart_put(lft_rgt);



			noDataCount = 0;
		
		} else {
			//start a countdown
			noDataCount++;

			nrf_gpio_pin_set(LED_left);
			nrf_gpio_pin_set(LED_tail);
			nrf_gpio_pin_set(LED_right);
			nrf_gpio_pin_set(LED_head);

			// added/commented out on Dec 7 2014 by Nick to stop interjecting stop commands while getting drive commands from the media center
			// if ( (tracking) || (BLE_driving) )
			// {
			// 	fwd_bwd = 0;
			// 	lft_rgt = 0;

			// 	simple_uart_put(0x78);
			// 	simple_uart_put(fwd_bwd);
			// 	simple_uart_put(lft_rgt);
			// }



		}

		if(!nrf_gpio_pin_read(Switch))
		{
			//button pressed
			nrf_gpio_pin_set(RGB_RED);
			nrf_gpio_pin_clear(RGB_GREEN);
			nrf_gpio_pin_clear(RGB_BLUE);

			simple_uart_put(0x50);
			simple_uart_put(0x00);
			simple_uart_put(0x00);



		} else {

			nrf_gpio_pin_set(RGB_RED);
			nrf_gpio_pin_set(RGB_GREEN);
			nrf_gpio_pin_set(RGB_BLUE);				
		}



		power_manage();


	};
}
