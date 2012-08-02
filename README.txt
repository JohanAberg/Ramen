	Ramen2
	------

	Ramen2 is a rewrite of Ramen, keeping the good things 
	of the original and trying to remove it's limitations.

	Ideas
	-----

		- Write the UI and other parts of the app in Python.
		The time of C++ user interfaces is over. It's simply not worth it.

		- Hierarchical nodes ( Houdini style).

		- Support processing other kinds of data, in addition to images. 
		Try to be as generic as possible.

		- Decouple the types of nodes with the results they produce. 
		A common error on other node based softwares including Ramen1.

	                - No image nodes, no geom nodes, ...
        	        - Any node should produce what it wants any time, 
			possibly depending on it's inputs.

		- Use the GFlops. In a typical modern machine, 75% of the performance 
		is in the GPU. Does it makes sense to only use 25% of the machine?

		- Borrow ideas from functional programming. In particular, 
		keep things as stateless as possible. Traditional object oriented 
		code with shared references does not work well on modern machines.
