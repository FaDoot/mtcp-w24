# Running the P1 Simulator
The P1 Simulator is an extremely basic visualizer that you can use to see the updates made to the system's pose.
It is composed of an HTML and JavaScript webpage that connects to a Python bridge script. The bridge and the webpage
connect via WebSockets, which implement a communication protocol on top of TCP/IP just like MessageSockets - Only much 
more complicated. Your C++ code can interact with the simulator by connecting to the Python bridge on port 8002 with a 
`ClientMessageSocket`, and sending JSON formatted `Pose2d` messages. We call the Python script a bridge because its only
function is to communicate between two other programs in different ways.

1. Go into `/simulator` and run `python3 -m http.server`. This serves the simulator to your browser on port 8000.
2. Open a new terminal and run `python3 bridge.py` to start the bridge.
3. Type `localhost:8000` into your browser search bar to load the web page. You should see output from the bridge that it has connected.
4. Run any C++ program with a `ClientMessageSocket` that connects to the bridge on port 8002.

The webpage attempts to connect to the bridge when it is loaded or refreshed. (It has a *client* websocket.) The bridge 
should be in its initial state with the output "refresh the P1 simulator in your browser" when the webpage attempts to connect. 
No C++ programs should attempt to connect to the bridge before the bridge and webpage are connected.

If you want to verify that the bridge and webpage are working, you can run an executable built from `test_manual/test_bridge_and_simulator`. 
If your `ClientMessageSocket`, `Pose2d` message, and `Json` object are all implemented correctly, you should see the cylinder bot 
spin on the webpage.
