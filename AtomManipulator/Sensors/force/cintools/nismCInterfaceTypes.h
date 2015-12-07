  #ifndef ___nismCInterfaceTypes_h___
#define ___nismCInterfaceTypes_h___


/*Handle to a resource or operation*/
typedef long nism_Handle;

/*Property Enumeration Values*/
	/*
	* Name: Circular Arc Move
	* Description: Performs a circular arc move on a coordinate. A circular arc defines an
	*              arc in the xy plane of a two- or three-dimensional coordinate space. 
	*/
	#define nism_ArcMoveModeCircular 0xB1000C
	
	/*
	* Name: Helical Arc Move
	* Description: Performs a helical arc move on a coordinate. A helical arc is an arc in
	*              three-dimensional space that consists of a circle in the xy plane and synchronized
	*              linear travel on the z-axis. 
	*/
	#define nism_ArcMoveModeHelical 0xB1000E
	
	/*
	* Name: Spherical Arc Move
	* Description: Performs a spherical arc move on a coordinate. Spherical arcs are defined
	*              as a circular arc in the x'y' plane of a transformed coordinate space,
	*              x'y'z'. The transformed coordinate space is defined in orientation only,
	*              with no absolute position offset. Orientation of the transformed coordinate
	*              space is with respect to the xyz coordinate space and is defined in terms
	*              or roll, yaw, and pitch angles. 
	*/
	#define nism_ArcMoveModeSpherical 0xB1000D
	
	/*
	* Name: Direct Coupling Camming
	* Description: Configures a camming operation using direct coupling between the master
	*              axis and slave axis. During direct coupling, the move constraints of the
	*              slave axis are not taken into account and the velocity of the slave axis
	*              is directly controlled by the master axis. 
	*/
	#define nism_CammingModeDirectCoupling 0xB1003C
	
	/*
	* Name: Continuous Contour Move
	* Description: Performs a move using the data provided in the table or the Points[][]
	*              input (synchronous operations only) until no more data is available. The
	*              table data can be dynamically updated using the Update Points or Update
	*              Points Interleaved methods. When defining a continuous contour move, you
	*              do not specify move constraints for the move. Instead, the position points
	*              are provided with respect to time and NI SoftMotion implicitly defines
	*              the move constraints taking the specified spline type into account.
	*/
	#define nism_ContourMoveModeContinuous 0xB10012
	
	/*
	* Name: Finite Contour Move
	* Description: Performs a move using the data provided in the table or the Points[][]
	*              input (synchronous operations only) one time. You cannot update the data
	*              in the table during execution when performing a finite move. When defining
	*              a finite contour move, you do not specify move constraints for the move.
	*              Instead, the position points are provided with respect to time and NI SoftMotion
	*              implicitly defines the move constraints taking the specified spline type
	*              into account.
	*/
	#define nism_ContourMoveModeFinite 0xB10010
	
	/*
	* Name: Repetitive Contour Move
	* Description: Performs a move using the data provided in the table or the Points[][]
	*              input (synchronous operations only) the number of times indicated by the
	*              iterations input. You cannot update the data in the table during execution
	*              when performing a repetitive move. When defining a repetitive contour move,
	*              you do not specify move constraints for the move. Instead, the position
	*              points are provided with respect to time and NI SoftMotion implicitly defines
	*              the move constraints taking the specified spline type into account.
	*/
	#define nism_ContourMoveModeRepetitive 0xB10011
	
	/*
	* Name: S-Curve Contour Move
	* Description: Performs a move using the user-specified position points and move constraints.
	*              S-Curve moves give you maximum control over the position of the move after
	*              each segment of the move. S-Curve moves specify a series of move segments
	*              by providing several move points for each axis using the contour move Points[][]
	*              input. The S-Curve method uses this data and applies the specified move
	*              constraints to the move. The position profile can contain one or multiple
	*              axes.
	*/
	#define nism_ContourMoveModeSCurve 0xB100E7
	
	/*
	* Name: Velocity-Time Contour Move
	* Description: Performs a move using the user-specified position, velocity, and time points.
	*              Velocity-Time moves give you maximum control over the velocity of the move
	*              at certain times throughout the move. With a Velocity-Time move you specify
	*              a series of position points for each axis using the contour move Points[][]
	*              input. In addition, with Velocity-Time mode you specify both the time,
	*              in s, each move segment takes using the Time[] input and the velocity at
	*              which all axes are traveling at the end of each segment using the Velocity[]
	*              input.
	*/
	#define nism_ContourMoveModeVelocityTime 0xB100EF
	
	/*
	* Name: Gearing Encoder Master
	* Description: Assigns an encoder as the master in a gearing or camming operation. When
	*              an encoder is assigned as the master, the slave axis follows the feedback
	*              generated by the encoder. If the encoder detects movement, the slave moves
	*              proportionally to information returned by the encoder. For example, if
	*              you twist the master axis connected to the encoder, the slave axis also
	*              turns because it is using the position information gathered by the encoder.
	*/
	#define nism_EncoderMaster 0xB10055
	
	/*
	* Name: Gearing Trajectory Master
	* Description: Assigns the trajectory generator of an axis as the master in a gearing
	*              or camming operation. When an axis is assigned as the master, its trajectory
	*              generator output (not its feedback position) is used as the master position
	*              command. For example, if you manually move the master axis, the slave axis
	*              does not move because the trajectory generator of the master axis is not
	*              active.
	*/
	#define nism_TrajectoryMaster 0xB10054
	
	/*
	* Name: Direct Coupling Gearing
	* Description: Configures a gearing operation using direct coupling between the master
	*              axis and slave axis. During direct coupling, the move constraints of the
	*              slave axis are not taken into account and the velocity of the slave axis
	*              is directly controlled by the master axis. 
	*/
	#define nism_GearingModeDirectCoupling 0xB10034
	
	/*
	* Name: Blend After Delay
	* Description: Start the next move after a programmed delay time between the end of the
	*              previous move and the start of the next move. Set the delay using Blend
	*              Delay.
	*/
	#define nism_BlendModeAfterDelay 0xA10028
	
	/*
	* Name: Blend At Deceleration
	* Description: Blend moves by superimposing the deceleration profile of the previous move
	*              with the acceleration profile of the next move. 
	*/
	#define nism_BlendModeAtDeceleration 0xA10026
	
	/*
	* Name: Blend At Profile Complete
	* Description: Blend moves by starting the next move at the exact point when the previous
	*              move ended. 
	*/
	#define nism_BlendModeAtProfileComplete 0xA10027
	
	/*
	* Name: Single Position Capture
	* Description: Enables a single position capture event  
	*/
	#define nism_PositionCaptureModeSingle 0xB100BF
	
	/*
	* Name: Periodic Continuous Position Compare
	* Description: Continuously executes position compare outputs until the Disable method
	*              is executed. The first position compare output is interpreted with respect
	*              to an origin or zero position, and all other position compare outputs are
	*              interpreted with respect to the specified position compare period. 
	*/
	#define nism_PositionCompareModePeriodicContinuous 0xB1002D
	
	/*
	* Name: Single Absolute Position Compare
	* Description: Enables a single position compare output which is interpreted with respect
	*              to an origin or zero position. 
	*/
	#define nism_PositionCompareModeSingleAbsolute 0xB1002A
	
	/*
	* Name: Single Relative Position Compare
	* Description: Enables a single position compare output which is interpreted with respect
	*              to the encoder position at the time the position compare operation is enabled.
	*/
	#define nism_PositionCompareModeSingleRelative 0xB1002B
	
	/*
	* Name: Absolute Reference Move
	* Description: Performs a search for the mechanical zero angle of an absolute feedback
	*              device using the specified move constraints. 
	*/
	#define nism_ReferenceMoveModeAbsolute 0xC10002
	
	/*
	* Name: Center Reference Move
	* Description: Performs a search for the center point between the forward and reverse
	*              limits on an axis using the specified move constraints. 
	*/
	#define nism_ReferenceMoveModeCenter 0xB1001D
	
	/*
	* Name: Forward Limit Reference Move
	* Description: Performs a search for the forward limit on an axis using the specified
	*              move constraints. When the search operation initiates the axis starts moving
	*              in the direction of the limit. When the axis encounters the limit in that
	*              direction the search completes and done returns TRUE.
	*/
	#define nism_ReferenceMoveModeForwardLimit 0xB1001E
	
	/*
	* Name: Home Reference Move
	* Description: Searches for the home input or switch on an axis using the specified move
	*              constraints. You can configure the search direction, the edge (forward
	*              or reverse) of the home switch to stop on, and the desired travel direction
	*              when you approach the specified home edge.
	*/
	#define nism_ReferenceMoveModeHome 0xB1001B
	
	/*
	* Name: Index Reference Move
	* Description: Performs a search for the encoder index signal on an axis using the specified
	*              move constraints. The Find Index routine is accurate to one encoder count
	*              and provides a much more repeatable reference than using just a limit or
	*              home input edge. The Index method can be executed after a Home, Center,
	*              Forward Limit, or Reverse Limit operation. With this procedure, the home
	*              input only needs to be accurate enough to repeatedly locate the axis within
	*              the same encoder revolution or index period. Then you can use the Index
	*              method to find a unique instance of the index.
	*/
	#define nism_ReferenceMoveModeIndex 0xB1001C
	
	/*
	* Name: Reverse Limit Reference Move
	* Description: Performs a search for the reverse limit on an axis using the specified
	*              move constraints. When the search operation initiates the axis starts moving
	*              in the direction of the limit. When the axis encounters the limit in that
	*              direction the search completes and done returns TRUE.
	*/
	#define nism_ReferenceMoveModeReverseLimit 0xB1001F
	
	/*
	* Name: Catmull-Rom Spline Algorithm
	* Description: Uses a Catmull-Rom spline to calculate setpoints. The Catmull-Rom spline
	*              algorithm guarantees that the generated spline path passes through the
	*              provided points.
	*/
	#define nism_SplineTypeCatmullRom 0xA300E9
	
	/*
	* Name: Cubic B Spline Algorithm
	* Description: Uses a cubic B spline algorithm to calculate setpoints. The cubic B spline
	*              algorithm guarantees smoothness, so the generated spline path does not
	*              necessarily pass through the provided points.
	*/
	#define nism_SplineTypeCubicB 0xA300E8
	
	/*
	* Name: Disable Drive Stop
	* Description: Stops the axis or coordinate by disabling the drive and switching all outputs
	*              to their user-specified safe states, if the hardware supports safe states.
	*              On servo axes, a disable drive stop disables the control loop and zeros
	*              the output DAC, allowing frictional forces alone to stop the motion. On
	*              stepper axes, a disable drive stop ceases the stepper pulse generation
	*              as soon as NI SoftMotion receives the command. On both axis types, there
	*              is no trajectory profile during a disable drive stop. 
	*/
	#define nism_StopModeDisableDrive 0xB10062
	
	/*
	* Name: Decelerate Stop
	* Description: Commands the axis or coordinate to immediately start decelerating using
	*              the deceleration and deceleration jerk values specified for the move in
	*              progress. The final stop position depends on the deceleration values. The
	*              axis or coordinate does not reach the target position.
	*/
	#define nism_StopModeDecelerate 0xB10060
	
	/*
	* Name: Immediate Stop
	* Description: Stops the axis or coordinate by issuing a controlled hard stop.  
	*/
	#define nism_StopModeImmediate 0xB10061
	
	/*
	* Name: Absolute Straight-Line Move
	* Description: Performs a straight-line move by interpreting the target position with
	*              respect to an origin or zero position. The origin is typically set using
	*              the Reset Position function at a home switch, end of travel limit switch,
	*              or encoder index position. An absolute position move uses the specified
	*              acceleration, deceleration, jerk, and velocity to complete a trajectory
	*              profile with an ending position equal to the loaded absolute target position.
	*/
	#define nism_StraightLineMoveModeAbsolute 0xA10029
	
	/*
	* Name: Polyline Move
	* Description: Executes the series of straight-line moves specified in the Position[][]
	*              input. Each move uses the specified set of move constraints. 
	*/
	#define nism_StraightLineMoveModePolyline 0xB100E5
	
	/*
	* Name: Relative Straight-Line Move
	* Description: Performs a straight-line move by interpreting the target position with
	*              respect to the actual position at the time that Start is run. A relative
	*              position move uses the specified values of acceleration, deceleration,
	*              jerk, and velocity to complete a trajectory profile with an ending position
	*              equal to the sum of the loaded relative target position and the starting
	*              position.
	*/
	#define nism_StraightLineMoveModeRelative 0xA10030
	
	/*
	* Name: Relative To Captured Position Straight-Line Move
	* Description: Performs a straight-line move in which the target position is interpreted
	*              with respect to the last captured position or the last found Index position,
	*              whichever has occurred most recently. A relative to capture move uses the
	*              specified values of acceleration, deceleration, jerk, and velocity to complete
	*              a trajectory profile with an ending position equal to the sum of the loaded
	*              relative target position and the starting position.
	*/
	#define nism_StraightLineMoveModeRelativeToReference 0xA10031
	
	/*
	* Name: Velocity Straight-Line Move
	* Description: Executes the straight-line move at the specified velocity until a Stop
	*              Move is commanded, a limit is encountered or you execute a move at a new
	*              velocity. You do not specify target positions in velocity mode. The direction
	*              of motion is determined by the sign of the specified velocity.
	*/
	#define nism_StraightLineMoveModeVelocity 0xA10032
	
	/*
	* Name: Continuous Sampled Read
	* Description: Reserved 
	*/
	#define nism_TimeSampledReadModeContinuous 0xB100A7
	
	/*
	* Name: Finite Sampled Read
	* Description: Reserved 
	*/
	#define nism_TimeSampledReadModeFinite 0xB100A6


/*Name: nism_OperationType
* Description: Describes an operation
* Works with the following functions: nismCreateOperation
*/
typedef enum
{
	   /*
	   * Name: Arc Move
	   * Description: Performs a circular, spherical, or helical arc move on a coordinate. An
	   *              arc move produces motion in a circular shape using a radius and other parameters
	   *              you specify. The type of arc to perform changes based on the Arc Move Mode.
	   */
	   nism_ArcMove = 0xB10008,
	
	   /*
	   * Name: Camming
	   * Description: Configures an axis for camming operations. Camming is used in applications
	   *              where the slave axis follows a non-linear profile from a master device.
	   *               The type of camming operation changes based on the Camming Mode.
	   */
	   nism_Camming = 0xB10037,
	
	   /*
	   * Name: Contour Move
	   * Description: Performs a contour move on an axis or coordinate resource. A contour move
	   *              is expressed as a series of positions that NI SoftMotion uses to extrapolate
	   *              a smooth curve. These positions can be stored in a table or provided in
	   *              the Points[][] input (synchronous operations only). Each point in the move
	   *              is interpreted as an absolute position using the starting point of the
	   *              move as a temporary zero position. The type of contour move changes based
	   *              on the Contour Mode.
	   */
	   nism_ContourMove = 0xB10009,
	
	   /*
	   * Name: Gearing
	   * Description: Configures an axis for gearing operations. Gearing synchronizes the movement
	   *              of a slave axis to the movement of a master device, which can be an encoder
	   *              or the trajectory of another axis. The type of gearing operation to perform
	   *              changes based on the Gearing Mode.
	   */
	   nism_Gearing = 0xB1002E,
	
	   /*
	   * Name: Profile
	   * Description: Specifies a motion profile item for use when generating offline trajectories.
	   *              A motion profile is represented as a set of positions paired with a delta
	   *              t. Profiles are generated as relative (implied start at origin). 
	   */
	   nism_Profile = 0xB100CE,
	
	   /*
	   * Name: Position Capture
	   * Description: Records encoder position based on an external input, such as the state
	   *              of a sensor. You can use the captured position to execute a move relative
	   *              to a captured position, or simply record the encoder position when the
	   *              capture event occurs.
	   */
	   nism_Capture = 0xA10003,
	
	   /*
	   * Name: Position Compare
	   * Description: Synchronizes external activities with the specified encoder position. When
	   *              the specified position is reached, a user-configurable pulse is executed.
	   *              The behavior of the position compare operation changes based on the Compare
	   *              Mode.
	   */
	   nism_Compare = 0xB10025,
	
	   /*
	   * Name: Find Reference Move
	   * Description: Performs a reference move, such as locating a home or limit position, on
	   *              an axis resource. Reference moves are used to initialize the motion system
	   *              and establish a repeatable reference position. The behavior of the move
	   *              changes based on the Reference Move Mode.
	   */
	   nism_FindReferenceMove = 0xB10019,
	
	   /*
	   * Name: Stop Move
	   * Description: Stops commanded movement on an axis or coordinate. The behavior of the
	   *              move changes based on the Stop Mode.
	   */
	   nism_StopMove = 0xB1005E,
	
	   /*
	   * Name: Straight-Line Move
	   * Description: Performs a straight-line move on an axis or coordinate resource. A straight-line
	   *              move connects two points on one or more axes. Straight-line moves can be
	   *              position-based or velocity-based. The behavior of the move changes based
	   *              on the Line Move Mode.
	   */
	   nism_StraightLineMove = 0xA10023,
	
	   /*
	   * Name: Time Sampled Read
	   * Description: Reserved 
	   */
	   nism_TimeSampledRead = 0xB100A3

}nism_OperationType;

/*Properties*/
/*Name: nism_PropString
* Description: Set or Get properties of type string
* Works with the following functions: nismSetPropString, nimsGetPropString
*/
typedef enum
{
	   /*
	   * Name: Camming Master Axis
	   * Description: Specifies the master axis to use for the camming operation. 
	   */
	   nism_CammingMasterAxis = 0xB10046,
	
	   /*
	   * Name: Gearing Master Axis
	   * Description: Specifies the master axis to use for the gearing operation. 
	   */
	   nism_GearingMasterAxis = 0xB1002F,
	
	   /*
	   * Name: Table
	   * Description: Specifies a reference to an NI SoftMotion table. Tables contain data used
	   *              with contouring and camming operations. Values are loaded from a file using
	   *              the Configure Table dialog box or created programmatically using Create
	   *              Table (contour moves only). 
	   */
	   nism_Table = 0xA10039

}nism_PropString;

/*Name: nism_PropDouble
* Description: Set or Get properties of type double
* Works with the following functions: nismSetPropDouble, nismGetPropDouble
*/
typedef enum
{
	   /*
	   * Name: Arc Move Linear Travel
	   * Description: Specifies the total linear travel distance of the arc in position Units.
	   *              Linear travel adjusts the distance between the endpoints of a helical arc.
	   *              A higher value for Linear Travel increases the vertical stretch of the
	   *              corkscrew aspect of the helix, while a value of 0 flattens the helix onto
	   *              one plane, forming a circle. 
	   */
	   nism_ArcMoveLinearTravel = 0xB10007,
	
	   /*
	   * Name: Arc Move Pitch
	   * Description: Specifies the pitch angle for the arc in degrees. When rotating through
	   *              the pitch angle, the y and y' axes stay aligned with each other, while
	   *              the x'z' plane rotates around them. The x, y, and z axes are determined
	   *              by the order the axes are added using the Configure Coordinate Space dialog
	   *              box.
	   */
	   nism_ArcMovePitch = 0xB10005,
	
	   /*
	   * Name: Arc Move Radius
	   * Description: Specifies the distance from the center of the arc to its edge. 
	   */
	   nism_ArcMoveRadius = 0xB10001,
	
	   /*
	   * Name: Arc Move Roll
	   * Description: Specifies the roll angle for the arc in degrees. When rotating through
	   *              the roll angle, the x and x' axes stay aligned with each other, while the
	   *              y'z' plane rotates around them. The x, y, and z axes are determined by
	   *              the order the axes are added using the Configure Coordinate Space dialog
	   *              box.
	   */
	   nism_ArcMoveRoll = 0xB10004,
	
	   /*
	   * Name: Arc Move Start Angle
	   * Description: Specifies the start angle for the arc move, in degrees. The range is 0
	   *              to 360, where angle 0 is along the positive x' axis and values increase
	   *              counterclockwise from the positive x' axis in the x'y' plane.
	   */
	   nism_ArcMoveStartAngle = 0xB10002,
	
	   /*
	   * Name: Arc Move Travel Angle
	   * Description: Specifies the total travel distance of the arc in degrees. For example,
	   *              a travel angle of 90 degrees executes a quarter-circle, a travel angle
	   *              of 360 degrees creates a full circle, and a travel angle of 720 degrees
	   *              creates two full circles. A positive travel angle creates counterclockwise
	   *              circular motion. A negative travel angle creates clockwise motion. 
	   */
	   nism_ArcMoveTravelAngle = 0xB10003,
	
	   /*
	   * Name: Arc Move Yaw
	   * Description: Specifies the yaw angle for the arc in degrees. When rotating through the
	   *              yaw angle, the z and z' axes stay aligned with each other, while the x'y'
	   *              plane rotates around them. The x, y, and z axes are determined by the order
	   *              the axes are added using the Configure Coordinate Space dialog box.
	   */
	   nism_ArcMoveYaw = 0xB10006,
	
	   /*
	   * Name: Camming Master Offset
	   * Description: Sets the position offset of the master axis. This input is used to start
	   *              the same camming profile in slightly different positions in the camming
	   *              interval.
	   */
	   nism_CammingMasterOffset = 0xB10039,
	
	   /*
	   * Name: Camming Slave Offset
	   * Description: Sets the position offset of the slave axis. This input is used to shift
	   *              the slave positions by the specified offset amount.
	   */
	   nism_CammingSlaveOffset = 0xB1003A,
	
	   /*
	   * Name: Contour Move Interval
	   * Description: Configures the time interval between each contour point, in seconds. The
	   *              value specified must be an integer multiple of the NI Scan Engine Scan
	   *              Period, configured on the Scan Engine Page of the RT Target Properties
	   *              dialog box.
	   */
	   nism_ContourMoveInterval = 0xE10007,
	
	   /*
	   * Name: Maximum Acceleration
	   * Description: Specifies the maximum acceleration of the move in Units/s2. Setting acceleration
	   *              to 0 (default) allows NI SoftMotion to automatically calculate the acceleration
	   *              using the specified Smoothing. If you specify a value for acceleration
	   *              but not deceleration NI SoftMotion assumes equal values for acceleration
	   *              and deceleration when calculating the move profile.
	   */
	   nism_MaxAcceleration = 0xA10016,
	
	   /*
	   * Name: Maximum Acceleration Jerk
	   * Description: Specifies the maximum acceleration jerk of the move in Units/s3. Jerk is
	   *              the derivative of acceleration (change of acceleration per unit time).
	   *              Setting acceleration jerk to 0 (default) allows NI SoftMotion to automatically
	   *              calculate the jerk using the specified Smoothing. If you specify a value
	   *              for acceleration jerk but not deceleration jerk NI SoftMotion assumes equal
	   *              values for acceleration and deceleration when calculating the move profile.
	   *              If you specify jerk values but not acceleration and deceleration values,
	   *              NI SoftMotion automatically calculates all values and uses the provided
	   *              jerk values as maximum limits.
	   */
	   nism_MaxAccelerationJerk = 0xA10018,
	
	   /*
	   * Name: Blend Delay
	   * Description: Specifies the delay time for a blended move using the After Delay blend
	   *              mode, in seconds, between the end of the previous move and the start of
	   *              the next move. The default value is 1 second.
	   */
	   nism_BlendDelay = 0xA10022,
	
	   /*
	   * Name: Maximum Deceleration
	   * Description: Specifies the maximum deceleration of the move in Units/s2. Setting deceleration
	   *              to 0 (default) allows NI SoftMotion to automatically calculate the deceleration
	   *              using the specified Smoothing. If you specify a value for deceleration
	   *              but not acceleration NI SoftMotion assumes equal values for acceleration
	   *              and deceleration when calculating the move profile.
	   */
	   nism_MaxDeceleration = 0xA10017,
	
	   /*
	   * Name: Maximum Deceleration Jerk
	   * Description: Specifies the maximum deceleration jerk of the move in Units/s3. Jerk is
	   *              the derivative of deceleration (change of deceleration per unit time).
	   *              Setting deceleration jerk to 0 (default) allows NI SoftMotion to automatically
	   *              calculate the jerk using the specified Smoothing. If you specify a value
	   *              for deceleration jerk but not acceleration jerk NI SoftMotion assumes equal
	   *              values for deceleration jerk and acceleration jerk when calculating the
	   *              move profile. If you specify jerk values but not acceleration and deceleration
	   *              values, NI SoftMotion automatically calculates all values and uses the
	   *              provided jerk values as maximum limits.
	   */
	   nism_MaxDecelerationJerk = 0xA10019,
	
	   /*
	   * Name: Smoothing Factor
	   * Description: Specifies amount of smoothing to apply to the move profile. Valid options
	   *              are between 0 (least smooth) and 1 (most smooth), with a default of 0.5.
	   *              The smoothing setting works with the specified move constraints to automatically
	   *              calculate unspecified move constraints.
	   */
	   nism_SmoothingFactor = 0xA100CC,
	
	   /*
	   * Name: Maximum Velocity
	   * Description: Specifies the maximum velocity of the move in Units/s. 
	   */
	   nism_MaxVelocity = 0xA10015,
	
	   /*
	   * Name: Compare Period
	   * Description: The distance between each position compare output. 
	   */
	   nism_ComparePeriod = 0xB10027,
	
	   /*
	   * Name: Compare Position
	   * Description: Specifies the position compare position trigger. How the position compare
	   *              position is interpreted depends on the Compare Mode as well as the actual
	   *              position at the time the position compare operation is enabled. 
	   */
	   nism_ComparePosition = 0xB10026,
	
	   /*
	   * Name: Profile Time Interval
	   * Description: Specifies or provides the time interval, in seconds, of the move profile.
	   *              The default is 0.001 seconds. This property is used when solving the move
	   *              profile using the Solve Profile method.
	   */
	   nism_ProfileTimeInterval = 0xB100D9,
	
	   /*
	   * Name: Approach Velocity
	   * Description: Specifies the approach velocity for the reference move, in Units/s. This
	   *              velocity is usually slower than the search velocity and is used once the
	   *              desired reference location is reached. This allows a smooth approach into
	   *              the reference location to minimize the effects of motion system windup
	   *              and/or backlash.
	   */
	   nism_ReferenceApproachVelocity = 0xB100A1,
	
	   /*
	   * Name: Search Distance
	   * Description: Specifies the total travel distance to search for an index position before
	   *              the move is aborted. 
	   */
	   nism_ReferenceSearchDistance = 0xB10024,
	
	   /*
	   * Name: Search Velocity
	   * Description: Specifies the search velocity for the reference move, in Units/s. This
	   *              velocity is used for the initial reference search.
	   */
	   nism_ReferenceSearchVelocity = 0xB100A2,
	
	   /*
	   * Name: Axis Target Position
	   * Description: Specifies the target position to move the axis to. Target position is interpreted
	   *              with respect to the Line Move Mode.
	   */
	   nism_AxisTargetPosition = 0xA10012,
	
	   /*
	   * Name: Sample Interval
	   * Description: Reserved 
	   */
	   nism_SampleInterval = 0xB100AB

}nism_PropDouble; 

/*Name: nism_PropLong
* Description: Set or Get properties of type long
* Works with the following functions: nismSetPropLong, nismGetPropLong
*/
typedef enum
{
	   /*
	   * Name: Arc Move Mode
	   * Description: The type of arc to perform. Valid options are circular, spherical, and
	   *              helical.
	   */
	   nism_ArcMoveMode = 0xB1000B,
	
	   /*
	   * Name: Camming Master Type
	   * Description: Specifies the axis resource to use as the camming master. Valid options
	   *              are Trajectory Master or Encoder Master.
	   */
	   nism_CammingMasterType = 0xB10038,
	
	   /*
	   * Name: Camming Mode
	   * Description: Specifies the behavior of the master and slave axes during the camming
	   *              operation. Valid value is Direct Coupling.
	   */
	   nism_CammingMode = 0xB1003B,
	
	   /*
	   * Name: Contour Move Iterations
	   * Description: Specifies the number of times to use the provided set of contour points.
	   */
	   nism_ContourMoveIterations = 0xB1000A,
	
	   /*
	   * Name: Contour Move Mode
	   * Description: Selects the type of contour move to perform. 
	   */
	   nism_ContourMoveMode = 0xB1000F,
	
	   /*
	   * Name: Contour Spline Type
	   * Description: Specifies the spline algorithm used to interpolate between contour points.
	   *              This spline type is used when the specified contour move interval is longer
	   *              than the NI Scan Engine Scan Period.
	   */
	   nism_SplineType = 0xB100EB,
	
	   /*
	   * Name: Position Loop Active Gain Set
	   * Description: Specifies the PID gain set to use. This input is 0-based. For example,
	   *              enter 0 to specify Gain Set 1.
	   */
	   nism_PositionLoopActiveGainSet = 0xC200A4,
	
	   /*
	   * Name: Gearing Master Type
	   * Description: Specifies the axis resource to use as the gearing master. Valid options
	   *              are Trajectory Master or Encoder Master.
	   */
	   nism_GearingMasterType = 0xB10032,
	
	   /*
	   * Name: Gearing Mode
	   * Description: Specifies the behavior of the master and slave axes during the gearing
	   *              operation. Valid value is Direct Coupling.
	   */
	   nism_GearingMode = 0xB10033,
	
	   /*
	   * Name: Gearing Ratio Denominator
	   * Description: Sets the gear ratio denominator of the slave relative to the master. The
	   *              denominator must be a positive value. For example, a ratio of 1/4 configures
	   *              the slave axis to travel 1/4 of the total distance specified by the master
	   *              axis target position.
	   */
	   nism_GearingRatioDenominator = 0xB10031,
	
	   /*
	   * Name: Gearing Ratio Numerator
	   * Description: Sets the gear ratio numerator of the slave relative to the master. The
	   *              numerator is a signed value to allow for both positive and negative gearing.
	   *              For example, a ratio of 1/4 configures the slave axis to travel 1/4 of
	   *              the total distance specified by the master axis target position.
	   */
	   nism_GearingRatioNumerator = 0xB10030,
	
	   /*
	   * Name: Index
	   * Description: Specifies the index of a resource attached to an axis, such as an encoder.
	   *              The index is 0-based. For example, an index of 0 specifies Encoder 0 and
	   *              an index of 1 specifies Encoder 1 (if available). 
	   */
	   nism_Index = 0xA10002,
	
	   /*
	   * Name: Line
	   * Description: Contains the line number. Refer to the Pin item in the Digital I/O page
	   *              of the Axis Configuration dialog box to determine what number to use for
	   *              this input.
	   */
	   nism_LineNumber = 0xB10087,
	
	   /*
	   * Name: Blend Mode
	   * Description: Specifies the blend mode for a move. Valid options are At Deceleration,
	   *              At Profile Complete (default), and After Delay.
	   */
	   nism_BlendMode = 0xA10020,
	
	   /*
	   * Name: Position Capture Mode
	   * Description: The operation mode for Position Capture. Valid value is Single.
	   */
	   nism_CaptureMode = 0xA10010,
	
	   /*
	   * Name: Compare Mode
	   * Description: Specifies the behavior of the position compare outputs. Valid options are
	   *              Single Absolute, Single Relative, and Periodic Continuous.
	   */
	   nism_PositionCompareMode = 0xB10029,
	
	   /*
	   * Name: Reference Move Mode
	   * Description: The type of reference move to perform. Valid options are Home, Index, Center,
	   *              Forward Limit, Reverse Limit, or Absolute.
	   */
	   nism_ReferenceMoveMode = 0xB1001A,
	
	   /*
	   * Name: Stop Mode
	   * Description: The type of stop to perform. Valid options are Decelerate, Immediate, and
	   *              Disable Drive.
	   */
	   nism_StopMode = 0xB1005F,
	
	   /*
	   * Name: Straight-Line Move Mode
	   * Description: The position mode for the move. Valid options are absolute, relative, relative
	   *              to capture, and velocity.
	   */
	   nism_StraightLineMoveMode = 0xA10014,
	
	   /*
	   * Name: Sample Mode
	   * Description: Reserved 
	   */
	   nism_SampleMode = 0xB100A5,
	
	   /*
	   * Name: Number Of Samples
	   * Description: Reserved 
	   */
	   nism_NumberOfSamples = 0xB100AC

}nism_PropLong;

/*Name: nism_PropBool
* Description: Set or Get properties of type bool
* Works with the following functions: nismSetPropBool, nismGetPropBool
*/
typedef enum
{
	   /*
	   * Name: Blend Enable
	   * Description: Smoothly join sequential moves. Enabling blend smoothly blends two move
	   *              segments on an axis, or coordinate. Valid options are At Deceleration,
	   *              At Profile Complete (default), and After Delay.
	   */
	   nism_BlendEnable = 0xA10025,
	
	   /*
	   * Name: Last Blend
	   * Description: Specifies the last move in a series of blended moves. The default value
	   *              is FALSE. Set to TRUE to specify the last move in a series of blended operations.
	   */
	   nism_LastBlend = 0xA10021,
	
	   /*
	   * Name: Enable Axis
	   * Description: Enables or disables the axis. Set to TRUE to enable the axis and FALSE
	   *              to disable the axis. When the axis is disabled, all outputs are set to
	   *              the specified safe state set in the Axis Configuration dialog box.
	   */
	   nism_EnableAxis = 0xB100BB,
	
	   /*
	   * Name: Enable Drive
	   * Description: Activates or deactivates the Drive Enable signal. Set to TRUE to enable
	   *              the drive, and FALSE to disable the drive. Configure the Drive Enable signal
	   *              using the Drive Enable page of the Axis Configuration dialog box.
	   */
	   nism_EnableDrive = 0xB100BC,
	
	   /*
	   * Name: Start Final Approach in Forward Direction
	   * Description: Specifies the direction the axis moves before stopping on the home or limit.
	   *              Select TRUE to approach from the forward direction or FALSE to approach
	   *              from the reverse direction. Approach forward means in the direction of
	   *              the limit, so for the reverse limit, approach forward actually means approach
	   *              toward the reverse limit. This approach direction feature is used to minimize
	   *              the effects of motion system windup and/or backlash.
	   */
	   nism_ReferenceApproachForward = 0xB10023,
	
	   /*
	   * Name: Stop Reference Move On Forward Edge
	   * Description: Specifies which edge of the home switch to stop on. Select TRUE to stop
	   *              on the forward edge of the Home switch and FALSE to stop on the reverse
	   *              edge of the Home switch.
	   */
	   nism_ReferenceStopOnForwardEdge = 0xB10021,
	
	   /*
	   * Name: Start Search In Forward Direction
	   * Description: Specifies in which direction to begin the reference move. Select TRUE to
	   *              search in the forward direction and FALSE to search in the reverse direction.
	   */
	   nism_ReferenceSearchForward = 0xB10022

}nism_PropBool;

/*Name: nism_PropDoubleArray
* Description: Set or Get properties of type double[]
* Works with the following functions: nismSetPropDoubleArray, nismGetPropDoubleArray
*/
typedef enum
{
	   /*
	   * Name: Time[]
	   * Description: Specifies time intervals to use in a Velocity-Time move, in seconds. 
	   */
	   nism_VTContourTimeArray = 0xB100EE,
	
	   /*
	   * Name: Maximum Velocity[]
	   * Description: Specifies velocity points to use in a Velocity-Time move. 
	   */
	   nism_VTContourVelocityArray = 0xB100ED,
	
	   /*
	   * Name: Curvature[]
	   * Description: Returns the curvature solution at each position setpoint along the motion
	   *              profile with the specified move constraints applied to the solution. The
	   *              curvature is the amount by which the profile deviates from being flat,
	   *              or straight, and is defined as the reciprocal of the radius of the circle
	   *              that most closely approximates the curve at the given profile point.
	   */
	   nism_CurvatureArray = 0xB100F3,
	
	   /*
	   * Name: Velocity[]
	   * Description: Returns the profile velocity as a point-by-point versus time array of the
	   *              velocity as it was solved. 
	   */
	   nism_ProfileVelocityArray = 0xB100F5,
	
	   /*
	   * Name: Coordinate Target Position
	   * Description: Specifies the target position of a coordinate straight-line move. Load
	   *              positions for each axis in a 1D array in the order that they were added
	   *              using the Configure Coordinate Space dialog box. Target positions are interpreted
	   *              with respect to the Line Move Mode.
	   */
	   nism_CoordinateTargetPosition = 0xA10013

}nism_PropDoubleArray;

/*Name: nism_PropDoubleArray2D
* Description: Set or Get properties of type double[][]
* Works with the following functions: nismSetPropDoubleArray2D, nismGetPropDoubleArray2D
*/
typedef enum
{
	   /*
	   * Name: Contour Points
	   * Description: Contains a 2D array of position points to use as initial points in a contour
	   *              move or to update the contour table during a continuous contour move. For
	   *              axis contour moves, the array contains axis contour points in the first
	   *              row of the array. For coordinate contour moves the array contains the axis
	   *              positions for each axis in the coordinate in each row in the order added
	   *              using the Configure Coordinate Space dialog box when you start a contour
	   *              move.
	   */
	   nism_ContourMovePointsArray = 0xB100E8,
	
	   /*
	   * Name: Profile[][]
	   * Description: Returns the position setpoints for each axis. Profile[ ][ ] output depends
	   *              on the specified Tolerance value.-->
	   */
	   nism_ProfileArray = 0xB100D7,
	
	   /*
	   * Name: Polyline Position[][]
	   * Description: Specifies a series of position points for use in a multiple segment move
	   *              using the Polyline straight-line move mode. Enter position points for multiple
	   *              axes in separate rows, with each row representing a different axis.
	   */
	   nism_PolylinePointsArray = 0xB100D8

}nism_PropDoubleArray2D;


/*Data*/
/*Name: nism_WriteDataDouble
* Description: Write data of type double
* Works with the following functions: nismWriteDataDouble
*/
typedef enum
{
	   /*
	   * Name: Analog Data
	   * Description: Contains the state of the specified analog line associated with the selected
	   *              axis resource. 
	   */
	   nism_WriteAnalogData = 0xB1007F,
	
	   /*
	   * Name: Axis Reset Position
	   * Description: Contains the reset position to move the axis to. 
	   */
	   nism_WriteAxisResetPosition = 0xB10089,
	
	   /*
	   * Name: Write Setpoint
	   * Description: Specifies the new setpoint position. 
	   */
	   nism_WriteSetpoint = 0xB10081,
	
	   /*
	   * Name: Write Velocity Override
	   * Description: Specifies the percentage by which the operating velocity on an axis or
	   *              coordinate is scaled. For example, a value of 25 specifies a velocity override
	   *              of 25% of the commanded velocity. All velocity changes use the specified
	   *              values of acceleration, deceleration, and jerk to smoothly transition the
	   *              velocity to its new value.
	   */
	   nism_WriteVelocityOverridePercentage = 0xB10082

}nism_WriteDataDouble;

/*Name: nism_ReadDataDouble
* Description: Read data of type double
* Works with the following functions: nismReadDataDouble
*/
typedef enum
{
	   /*
	   * Name: Analog Data
	   * Description: Contains the state of the specified analog line associated with the selected
	   *              axis resource. 
	   */
	   nism_ReadAnalogData = 0xB1007F,
	
	   /*
	   * Name: Captured Position
	   * Description: Contains the position registered when the Position Capture operation occurs.
	   *              This is only valid for single position captures.
	   */
	   nism_ReadCapturedPosition = 0xA10006,
	
	   /*
	   * Name: Axis Position
	   * Description: Returns the position of the axis. For open-loop stepper axes, data is returned
	   *              as position information in number of steps generated. For closed-loop stepper
	   *              and servo axes, the primary feedback position is returned.
	   */
	   nism_ReadAxisPosition = 0xA10033,
	
	   /*
	   * Name: Commanded Acceleration
	   * Description: Returns the axis or coordinate acceleration commanded by the trajectory
	   *              generator in Units/s2. 
	   */
	   nism_ReadCommandedAcceleration = 0xB1009A,
	
	   /*
	   * Name: Commanded Axis Position
	   * Description: Returns the instantaneous trajectory position of the axis. This output
	   *              is commanded pos.[] out when you use a coordinate resource and contains
	   *              position information for each axis in the coordinate in a one-dimensional
	   *              array.
	   */
	   nism_ReadCommandedAxisPosition = 0xB10097,
	
	   /*
	   * Name: Commanded Velocity
	   * Description: Returns the axis or coordinate velocity commanded by the trajectory generator
	   *              in Units/s. This velocity is not the same as the velocity returned by the
	   *              Data method, which is the actual axis velocity.
	   */
	   nism_ReadCommandedVelocity = 0xB10099,
	
	   /*
	   * Name: Encoder Position
	   * Description: Returns the incremental count value of the selected encoder. 
	   */
	   nism_ReadEncoderPosition = 0xB1006F,
	
	   /*
	   * Name: Encoder Velocity
	   * Description: Returns the velocity of the encoder, in Units/s. Specify the units for
	   *              your application using the Encoder page of the Axis Configuration dialog
	   *              box.
	   */
	   nism_ReadEncoderVelocity = 0xB10090,
	
	   /*
	   * Name: Captured Index Position
	   * Description: Returns the index position from the last Find Index search. 
	   */
	   nism_ReadCapturedIndexPosition = 0xB10070,
	
	   /*
	   * Name: Position Error
	   * Description: Returns the position error of the axis. Position error is the difference
	   *              between the instantaneous trajectory position and the primary feedback
	   *              position.
	   */
	   nism_ReadPositionError = 0xB1006D,
	
	   /*
	   * Name: Steps Generated
	   * Description: Returns the total number of steps generated for the axis. When resource
	   *              is a servo axis, steps generated returns 0.
	   */
	   nism_ReadStepsGenerated = 0xB10096,
	
	   /*
	   * Name: Torque
	   * Description: Returns the torque of the motor. Torque is calculated using the drive command
	   *              output.
	   */
	   nism_ReadTorque = 0xB1006C,
	
	   /*
	   * Name: Velocity
	   * Description: Returns the velocity of the axis or coordinate in Units/s. When you read
	   *              velocity for an axis, velocity contains the filtered velocity in Units/s.
	   *              When resource is a coordinate, the velocity is the filtered coordinate
	   *              velocity for the coordinate move.
	   */
	   nism_ReadVelocity = 0xA10035

}nism_ReadDataDouble;

/*Name: nism_ReadDataLong
* Description: Read data of type long
* Works with the following functions: nismReadDataLong
*/
typedef enum
{
	   /*
	   * Name: Contour Current Iteration
	   * Description: Returns the number of table iterations the repetitive contour move has
	   *              performed. 
	   */
	   nism_ReadContourMoveIteration = 0xB10016,
	
	   /*
	   * Name: Contour Space Available
	   * Description: Returns the amount of data space available in the contour buffer as the
	   *              total amount of individual points that can be written to the buffer. For
	   *              example space available returns 40 for a contour table containing points
	   *              for two axes with 20 remaining xy points.
	   */
	   nism_ReadContourMoveSpaceAvailable = 0xB10015,
	
	   /*
	   * Name: Modal Error
	   * Description: Contains the most recent NI SoftMotion modal error code. Refer to NI SoftMotion
	   *              Error Codes for error descriptions.
	   */
	   nism_ReadModalError = 0xC200A2

}nism_ReadDataLong;

/*Name: nism_WriteDataBool
* Description: Write data of type bool
* Works with the following functions: nismWriteDataBool
*/
typedef enum
{
	   /*
	   * Name: Contour Last Update
	   * Description: Specifies the last update to the continuous contour move. This value is
	   *              FALSE by default, indicating that more points will be provided. Not setting
	   *              last update to TRUE during a continuous contour move results in an error
	   *              when the contour buffer runs out of points.
	   */
	   nism_WriteContourMoveLastUpdate = 0xB10018,
	
	   /*
	   * Name: Contour Move Overwrite Points
	   * Description: Specifies whether to overwrite the points in the contour table or append
	   *              the new points after the old points. This value is FALSE by default. Set
	   *              overwrite to TRUE when calling Update Points or Update Points Interleaved
	   *              to reinitialize the table with new data before starting a move.
	   */
	   nism_WriteContourMoveOverwritePoints = 0xC2000C,
	
	   /*
	   * Name: Digital Data
	   * Description: Contains the state of the specified digital line associated with the selected
	   *              axis resource as Boolean data where TRUE is active and FALSE is inactive.
	   */
	   nism_WriteDigitalData = 0xB10080

}nism_WriteDataBool;

/*Name: nism_ReadDataBool
* Description: Read data of type bool
* Works with the following functions: nismReadDataBool
*/
typedef enum
{
	   /*
	   * Name: Digital Data
	   * Description: Contains the state of the specified digital line associated with the selected
	   *              axis resource as Boolean data where TRUE is active and FALSE is inactive.
	   */
	   nism_ReadDigitalData = 0xB10080

}nism_ReadDataBool;

/*Name: nism_WriteDataDoubleArray
* Description: Write data of type double[]
* Works with the following functions: nismWriteDataDoubleArray
*/
typedef enum
{
	   /*
	   * Name: Contour Points
	   * Description: Contains a 1D array of contour points for updating the contour table. For
	   *              axis contour moves, the array is a simple 1D array. For coordinate contour
	   *              moves, the contour points are interleaved with position points for each
	   *              axis in the order added using the Configure Coordinate Space dialog box.
	   */
	   nism_WriteContourMovePoints = 0xB10017,
	
	   /*
	   * Name: Coordinate Reset Position
	   * Description: Contains the reset positions for each axis in the coordinate, in a 1D array.
	   *              The axes do not move during a reset operation. Provide reset positions
	   *              for each axis in the order that they were added using the Configure Coordinate
	   *              Space dialog box.
	   */
	   nism_WriteCoordinateResetPosition = 0xB1008A

}nism_WriteDataDoubleArray;

/*Name: nism_ReadDataDoubleArray
* Description: Read data of type double[]
* Works with the following functions: nismReadDataDoubleArray
*/
typedef enum
{
	   /*
	   * Name: Commanded Coordinate Position
	   * Description: Returns the instantaneous trajectory position of each axis in the specified
	   *              coordinate. Position information is returned for each axis in the order
	   *              added using the Configure Coordinate Space dialog box.
	   */
	   nism_ReadCommandedCoordinatePosition = 0xB10098,
	
	   /*
	   * Name: Coordinate Position
	   * Description: Contains a 1D array of the positions of each axis in the coordinate. For
	   *              open-loop stepper axes, data is returned as position information in number
	   *              of steps generated. For stepper axes that use encoder feedback and servo
	   *              axes the primary feedback position is returned. Position information is
	   *              returned for each axis in the order that it was added using the Configure
	   *              Coordinate Space dialog box.
	   */
	   nism_ReadCoordinatePosition = 0xA10034,
	
	   /*
	   * Name: Sampled Data
	   * Description: Reserved 
	   */
	   nism_ReadSampledData = 0xB100A4

}nism_ReadDataDoubleArray;

/*Name: nism_ReadDataLongArray
* Description: Read data of type long[]
* Works with the following functions: nismReadDataLongArray
*/
typedef enum
{
	   /*
	   * Name: Fault Code[]
	   * Description: Contains all currently reported fault codes in an array. The array is user-allocated
	   *              and must be initialized before use so that internal memory allocation is
	   *              not required, which may introduce jitter into the system.
	   */
	   nism_ReadFaultCodes = 0xB100A0

}nism_ReadDataLongArray;

/*Name: nism_WriteDataDoubleArray2D
* Description: Read data of type double[][]
* Works with the following functions: nismWriteDataDoubleArray2D
*/
typedef enum
{
	   /*
	   * Name: Contour Points
	   * Description: Contains a 2D array of position points to use to update the contour table
	   *              during a continuous contour move. For axis contour moves, the array contains
	   *              axis contour points in the first row of the array. For coordinate contour
	   *              moves the array contains the axis positions for each axis in the coordinate
	   *              in each row in the order added using the Configure Coordinate Space dialog
	   *              box when you start a contour move.
	   */
	   nism_WriteContourMovePointsArrayUpdate = 0xB100EA

}nism_WriteDataDoubleArray2D;

/*Status*/
/*Name: nism_Status
* Description: Read status
* Works with the following functions: nismReadStatus
*/
typedef enum
{
	   /*
	   * Name: Aborted
	   * Description: Indicates that the given operation has been interrupted by another user
	   *              command and has finished executing unsuccessfully without error. If the
	   *              requested operation is interrupted, aborted remains TRUE as long as execute
	   *              is TRUE.
	   */
	   nism_Aborted = 0xA10007,
	
	   /*
	   * Name: Active
	   * Description: Indicates that all required resources have been sent the commands and the
	   *              requested operation is pending or in the process of execution. active remains
	   *              TRUE once execute is TRUE and until the requested operation finishes executing
	   *              and all resources used by the VI or function are freed.
	   */
	   nism_Active = 0xA10009,
	
	   /*
	   * Name: Axis Enabled
	   * Description: Specifies whether the axis is enabled. Returns TRUE when the axis is enabled,
	   *              and FALSE when the axis is not enabled.
	   */
	   nism_AxisEnabled = 0xA30002,
	
	   /*
	   * Name: Busy
	   * Description: Indicates that the given operation has not completed execution. busy is
	   *              TRUE once execute is TRUE and as long as the operation has not finished.
	   *              Status can be busy but not active if the operation is waiting for resources
	   *              to become available.
	   */
	   nism_Busy = 0xA10008,
	
	   /*
	   * Name: Done
	   * Description: Indicates that the given operation completed without error. Upon successful
	   *              completion, done remains TRUE as long as execute is TRUE.
	   */
	   nism_Done = 0xA10005,
	
	   /*
	   * Name: Drive Enabled
	   * Description: Returns TRUE when drive enable signal is active, enabling the drive, and
	   *              FALSE when the drive enable signal is not active, disabling the drive.
	   */
	   nism_DriveEnabled = 0xB1005A,
	
	   /*
	   * Name: Blend Is Complete
	   * Description: Returns TRUE if the previous blend is complete and the axis is ready to
	   *              receive the next blend command. 
	   */
	   nism_BlendComplete = 0xB10057,
	
	   /*
	   * Name: Drive Fault Is Active
	   * Description: Returns TRUE if the Drive Fault input is active. 
	   */
	   nism_DriveFaultActive = 0xB10091,
	
	   /*
	   * Name: Drive Ready Is Active
	   * Description: Returns TRUE if the Drive Ready signal is active. Configure the Drive Ready
	   *              signal using the Digital I/O page of the Axis Configuration dialog box.
	   */
	   nism_DriveReadyIsActive = 0xA10081,
	
	   /*
	   * Name: Fault Occurred
	   * Description: Returns TRUE if an NI SoftMotion Fault has occurred. When the system is
	   *              in a fault state, this output is the only output that contains valid information.
	   *              All other outputs are not guaranteed to contain valid information.
	   */
	   nism_FaultOccurred = 0xB100B3,
	
	   /*
	   * Name: Forward Limit Is Active
	   * Description: Returns TRUE if the forward limit signal is active. Configure limits using
	   *              the Limits & Home page of the Axis Configuration dialog box.
	   */
	   nism_ForwardLimitActive = 0xB10079,
	
	   /*
	   * Name: Forward Software Limit Is Active
	   * Description: Returns TRUE if the forward software limit is exceeded. Software limits
	   *              are configured using the Trajectory page of the Axis Configuration dialog
	   *              box.
	   */
	   nism_ForwardSoftwareLimitActive = 0xB1007C,
	
	   /*
	   * Name: Home Switch Is Active
	   * Description: Returns TRUE if the home signal is active. Configure the Home input using
	   *              the Limits & Home page of the Axis Configuration dialog box.
	   */
	   nism_HomeActive = 0xB1007B,
	
	   /*
	   * Name: Index Capture Occurred
	   * Description: Returns TRUE if the previous Find Index operation successfully located
	   *              the encoder index. 
	   */
	   nism_IndexFound = 0xB10071,
	
	   /*
	   * Name: Maximum Position Error Exceeded
	   * Description: Returns TRUE when the axis exceeds the programmed Position Error Limit.
	   *               Set the position error limit on the Stepper page of the Axis Configuration
	   *              dialog box for stepper axes, or on the Control Loop page for servo axes.
	   */
	   nism_PositionErrorLimitExceeded = 0xB1005C,
	
	   /*
	   * Name: Move Is Complete
	   * Description: Returns TRUE if the move complete status is TRUE. Configure move complete
	   *              options on the Trajectory tab of the Axis Configuration dialog box.
	   */
	   nism_MoveComplete = 0xB10056,
	
	   /*
	   * Name: Position Capture Occurred
	   * Description: Returns TRUE if the configured position capture operation has occurred.
	   */
	   nism_CaptureOccurred = 0xB10058,
	
	   /*
	   * Name: Position Compare Triggered
	   * Description: Returns TRUE if the configured position compare operation has occurred.
	   */
	   nism_CompareTriggered = 0xB10059,
	
	   /*
	   * Name: Profile Is Complete
	   * Description: Returns TRUE when the profile is complete, and FALSE when profile generation
	   *              is in progress. 
	   */
	   nism_ProfileComplete = 0xB1005B,
	
	   /*
	   * Name: Reverse Limit Is Active
	   * Description: Returns TRUE if the reverse limit signal is active. Configure limits using
	   *              the Limits & Home page of the Axis Configuration dialog box.
	   */
	   nism_ReverseLimitActive = 0xB1007A,
	
	   /*
	   * Name: Reverse Software Limit Is Active
	   * Description: Returns TRUE if the reverse software limit is exceeded. Configure software
	   *              limits using the Trajectory page of the Axis Configuration dialog box.
	   */
	   nism_ReverseSoftwareLimitActive = 0xB1007D,
	
	   /*
	   * Name: Target Reached Is Active
	   * Description: Returns TRUE if the target position on the specified axis is reached. 
	   */
	   nism_TargetReachedActive = 0xB10092,
	
	   /*
	   * Name: Velocity Threshold Exceeded
	   * Description: Returns TRUE when the velocity threshold is exceeded. Set the velocity
	   *              threshold on the Trajectory page of the Axis Configuration dialog box.
	   */
	   nism_VelocityThresholdExceeded = 0xA10080

}nism_Status;

#endif /*nismCInterfaceTypes_h*/


