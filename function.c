// Function to move the robot
// To be used in the programming project final version
// \param _xc X coordinate computed: new robot position
// \param _yc Y coordinate computed: new robot position
// \param xt X coordinate: old robot position
// \param yt Y coordinate: old robot position

void MoveRobot(int *_xc, int *_yc, int xt, int yt)
{
	int xc, yc;
	double angle;

	// just copy the values
	xc = *_xc;
	yc = *_yc;

	// calculate the angle
	angle = atan2((double)(yc-yt), (double)(xt-xc));
    
	// calculate the new position
	*_xc = floor(xc + cos(angle)+0.5);
	*_yc = floor(yc - sin(angle)+0.5);
}