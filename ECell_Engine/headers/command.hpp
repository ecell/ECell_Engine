/// <summary>
/// Abstract command class.
/// Introduced to handle command received from input in the console
/// in order to control the execution of the simulation loop.
/// </summary>

template <typename T>
class Command
{
protected:
	T* receiver;

public:
	/// <summary>
	/// Public interface to execute the action/code
	/// associated to the command.
	/// </summary>
	virtual void Execute() {};
	
	
	/// <summary>
	/// Sets the receiver of the command.
	/// This must be done early after the instantiation of the command.
	/// </summary>
	//virtual void SetReceiver(T* _receiver) {};
	
};