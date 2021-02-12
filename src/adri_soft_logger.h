// #ifdef TEST
#ifndef ADRI_SOFT_LOGGER_H
	#define ADRI_SOFT_LOGGER_H

	#include <arduino.h>
	#include <adri_tools_v2.h>

	extern char * adriLogger_buffer1;
	
	/**
	* @class	adriToolsLogger
	*
	* @brief	fournit des fonctionnalités utiles pour déboguer les croquis via "printf" ou "LITTLEFS"
	* @see		https://github.com/AdriLighting/adri_tools_v2
	*
	* @author	adrilighting
	* @date		05/02/2021 06:25:54
	*/
	class adriToolsLogger {
	private:
		/** \brief active/desactive le "printf" et "littfs" pour toute les region */
		boolean printer_display = true;

		/** \brief active  le "printf" et "littfs" uniquement pour le lvl selectionner pour toute les region */
		int printer_displayMod_pos 	= 0;

		/** \brief active/desactive le "littfs" pour toutes les region */
		boolean spiff_sav = false;

		/** \brief acitve/desactive le timestamp */
		boolean printer_timestamp = true;

		/** \brief activer/desactive le freeheap */
		boolean printer_heap = true;		

		/** \brief enplacement du doosier des log situé da la mem SPIFF */
		String spiff_folder = "/logger";

		/** \brief nom du fichier log situé da le dossier "spiff_folder" */
		String spiff_fileName = "";

	public:
		adriToolsLogger();
		adriToolsLogger(int size);
		void setup();

		void printer_timestamp_toggle();
		void printer_heap_toggle();

		String 	timeStamp();
		int 	strContains(char str, const char *sfind);

		void printer_display_toggle();
		
		boolean printer_displayMod(int region, int lvl);
		void printer_displayMod_set(int lvl);

		void printer_serial(boolean addLine, int region, int lvl, String func, char * buffer);
		

		void spiff_setup();
		void spiff_toggle();
		void spiff_readCurrent();
		void spiff_removeCurrent();
		void spiff_get(String & result);
		void spiff_addLine(String result);

		void serial_menu();

	};

	adriToolsLogger * adriToolsLogger_ptrGet();
	String adriToolLogger_serialMenu_cmd(String cmd, String value);
#endif // LOGGER_H
// #endif // LOGGER_H
