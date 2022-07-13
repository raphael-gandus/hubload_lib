

#include "MCP3202.h"



// L'ADC quantifie sur 12Bits, il y a donc 4096 valeurs possible de conversion.
// On a 1C tous les 12 pas en bit en sortie chez l'adc.
// Range du termometre: -40C -- 150C ; 0,1V a 2V.




void waitForSPIReady(expander_t *exp){
	
	time_t start, end;
	double attente = 0;
	start = clock();
	while((( expander_getAllPinsGPIO(exp) & (uint8_t)0b00111100) != 0b00111100 ))
	{
		end = clock();
		attente = 100*(double)(end - start) / (double)(CLOCKS_PER_SEC);
		if(attente > 5)
		{
			perror("Warning timeout: SPI busy tout les CS ne sont pas a 1");
			
		}
	}
}


/**
 * 
 * @brief  renvoi le code de conversion sur 12bitsde l'adc d'un channel precis et d'un adc precis
 * 
 * @param   channel le channel à lire 0 ou 1 
 * @param   cs le chip select de l'adc a lire (utiliser les constante de la lib expander-i2c.h (T_CS, PP_CS...))
 * 
 * @return  l'output code entre 0 et 4095 
 * 
 *  **/
int readAdc(int channel, uint8_t cs){

	unsigned int reData = -1;           
	
	uint8_t data[3] = {0};

		/* Setup du SPI pour l'adc */ 
	int fd = wiringPiSPISetupMode(0, 2000000, 0);
	if(fd < 0)
	{
		perror("Erreur de setup de SPI");
		return EXIT_FAILURE;
	}

	if(wiringPiSetup() < 0)
	{
		fprintf(stderr, "fonction %s: Unable to open i2c device: %s\n", __func__, strerror(errno));
	}
	data[0] = START_BIT;
	if(channel == 0){
		data[1] = ADC_CONFIG_SGL_MODE_MSBF_CN0;
	}
	else{
		data[1] = ADC_CONFIG_SGL_MODE_MSBF_CN1;
	}
	data[2] = DNT_CARE_BYTE;

	expander_t *exp = expander_init(0x27);

	

	// time_t start, end;
	double attente = 0;
	// start = clock();
	while((( expander_getAllPinsGPIO(exp) & (uint8_t)0b00111100) != 0b00111100 ))
	{
		usleep(1000000);
		attente++;
		if(attente > 5)
		{
			printf("fonction %s: Erreur timeout: SPI busy tout les CS ne sont pas a 1", __func__);
			close(fd);
			// exit(EXIT_FAILURE);
			return 0;
		}
	}

	// waitForSPIReady(exp);

	expander_setPinGPIO(exp,CP_DIS);
	// expander_setPinGPIO(exp, CP_CS);
	// expander_setPinGPIO(exp, PM_CS);
	// expander_setPinGPIO(exp, T_CS);
	// expander_setPinGPIO(exp, PP_CS);
	// cs concerné 0 uniquement lui les autres 1 
	expander_resetPinGPIO(exp, cs);
	
	usleep(1); //temps minimal avant transaction spi 100ns

	if(wiringPiSPIDataRW(0, data, 3) < 0){
		fprintf(stderr, "fonction %s: Error writing on SPI: %s\n", __func__, strerror(errno));
		expander_setPinGPIO(exp, cs);
		usleep(1); // temps minimal necessaire pour pouvoir redemander la valeur apres. ( TCSH = 500 ns)
		close(fd);

		expander_closeAndFree(exp);



		return 0;
	}
	// expander_setAndResetSomePinsGPIO(exp, ancienne_config);


	expander_setPinGPIO(exp, cs);
	usleep(1); // temps minimal necessaire pour pouvoir redemander la valeur apres. ( TCSH = 500 ns)


	reData = (((data[1] << 8) + data[2]) & MSBF_MASK);

	// printf("ADC retourne : %d\n", reData);
	expander_closeAndFree(exp);

	
	close(fd);
#ifdef DEBUG
	printf("The analog input value is \n");
	printf("Value at MCP3202 CH%d is: %d D \n", channel, reData);
#endif
	
	return reData;
}


/**
 * 
 * @brief  convertie l'output code du mcp3202 en tension en volt
 * 
 * @param   code l'output code compris entre 0 et 4095
 * 
 * @return  la tension en volt convertit a partir de l'output code
 * 
 *  **/
double toVolt(int code){

	if(code < 0){

		perror("ne peut pas convertir le code de sortie en volt valeur negative");
        return code;
	}
	return code*3.3/4095;
}

/**
 * 
 * @brief  convertie l'output code du mcp3202 en tension en mV
 * 
 * @param   code l'output code compris entre 0 et 4095
 * 
 * @return  la tension en mV convertit a partir de l'output code
 * 
 *  **/
double toMillivolt(int code){

	if(code < 0){

		perror("ne peut pas convertir le code de sortie en volt valeur negative");
        return code;
	}
	return code*3300.0/4096;
}
