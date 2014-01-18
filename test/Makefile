
./test_light: ./test_light.cpp ../interactions/LightSensor.cpp ../interactions/LightSource.cpp ../interactions/LightSourceFromAbove.cpp
	g++  -I /home/pedro/local/include/ $^ -o $@  -L /home/pedro/local/lib/ -lenki

run: ./test_light
	./test_light

clean:
	rm -f *~ test_light
