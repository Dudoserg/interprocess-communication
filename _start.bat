start Buyer.exe
pathping -h 1 -p 150 -q 1 -w 1 localhost
start Dispatcher.exe
pathping -h 1 -p 150 -q 1 -w 1 localhost
start Cook.exe
pathping -h 1 -p 150 -q 1 -w 1 localhost
start Fridge.exe