# EventPic
messing around with pic and events 
* pic is injected to a sample target process
* pic enters waiting state for 10 seconds , pops a box on timeout and waits again
* injector is responsible to signal the 'exit event' the pic is waiting on
* parameters are freed by the pic , pic is freed by the injector (pic could free itself using a rop chain)
* todo: use this template to instead of exit - migrate to another process 
