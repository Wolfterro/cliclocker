char fulldate[128];

static char* getdate(void)
{
    char * weekday[6];
    char * monthname[11];
    char day_sufix[4];

    time_t timenow = time(NULL);
    struct tm tm_timenow = *localtime(&timenow);

    // Defining day of the week
	// ========================
	weekday[0] = "Sunday";
	weekday[1] = "Monday";
	weekday[2] = "Tuesday";
	weekday[3] = "Wednesday";
	weekday[4] = "Thursday";
	weekday[5] = "Friday";
	weekday[6] = "Saturday";

	// Defining month name
	// ===================
	monthname[0] = "January";
	monthname[1] = "February";
	monthname[2] = "March";
	monthname[3] = "April";
	monthname[4] = "May";
	monthname[5] = "June";
	monthname[6] = "July";
	monthname[7] = "August";
	monthname[8] = "September";
	monthname[9] = "October";
	monthname[10] = "November";
	monthname[11] = "December";

	// Defining day sufix
	// ==================
	switch(tm_timenow.tm_mday) {
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

	snprintf(fulldate, sizeof fulldate, "[%s - %s %d%s, %d]", weekday[tm_timenow.tm_wday], monthname[tm_timenow.tm_mon], tm_timenow.tm_mday, day_sufix, tm_timenow.tm_year + 1900);

	return fulldate;
}