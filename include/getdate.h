char fulldate[128];

static char* getdate(void)
{
    int weekday_int;
    int monthname_int;
    int day_int;

    char weekday[16];
    char monthname[16];
    char day_sufix[4];

    time_t timenow = time(NULL);
    struct tm tm_timenow = *localtime(&timenow);

    // Defining day of the week
	// ========================
	weekday_int = tm_timenow.tm_wday;
	switch(weekday_int) {
		case 0:
			sscanf("Sunday", "%s", weekday);
			break;
		case 1:
			sscanf("Monday", "%s", weekday);
			break;
		case 2:
			sscanf("Tuesday", "%s", weekday);
			break;
		case 3:
			sscanf("Wednesday", "%s", weekday);
			break;
		case 4:
			sscanf("Thursday", "%s", weekday);
			break;
		case 5:
			sscanf("Friday", "%s", weekday);
			break;
		case 6:
			sscanf("Saturday", "%s", weekday);
			break;
		default:
			sscanf("N/A", "%s", weekday);
			break;
	}

	// Defining month name
	// ===================
	monthname_int = tm_timenow.tm_mon;
	switch(monthname_int) {
		case 0:
			sscanf("January", "%s", monthname);
			break;
		case 1:
			sscanf("February", "%s", monthname);
			break;
		case 2:
			sscanf("March", "%s", monthname);
			break;
		case 3:
			sscanf("April", "%s", monthname);
			break;
		case 4:
			sscanf("May", "%s", monthname);
			break;
		case 5:
			sscanf("June", "%s", monthname);
			break;
		case 6:
			sscanf("July", "%s", monthname);
			break;
		case 7:
			sscanf("August", "%s", monthname);
			break;
		case 8:
			sscanf("September", "%s", monthname);
			break;
		case 9:
			sscanf("October", "%s", monthname);
			break;
		case 10:
			sscanf("November", "%s", monthname);
			break;
		case 11:
			sscanf("December", "%s", monthname);
			break;
		default:
			sscanf("N/D ", "%s", monthname);
			break;
	}

	// Defining day sufix
	// ==================
	day_int = tm_timenow.tm_mday;
	switch(day_int) {
		case 1:
			sscanf("st", "%s", day_sufix);
			break;
		case 2:
			sscanf("nd", "%s", day_sufix);
			break;
		case 3:
			sscanf("rd", "%s", day_sufix);
			break;
		default:
			sscanf("th", "%s", day_sufix);
			break;
	}

	snprintf(fulldate, sizeof fulldate, "[%s - %s %d%s, %d]", weekday, monthname, tm_timenow.tm_mday, day_sufix, tm_timenow.tm_year + 1900);

	return fulldate;
}