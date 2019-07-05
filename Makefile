
.PHONY: check.deps
check.deps:
	@echo "Checking bundled libraries..."
	@kextlibs -xml /Users/pastel/Library/Developer/Xcode/DerivedData/AirPortAtheros9271-giyxvwyqrbunxsdenfzmxouaixip/Build/Products/Debug/AirPortAtheros9271.kext
	@echo "Return: $?"

.PHONY: check.load
check.load:
	@echo "Checking kext if loadable..."
	@sudo rm -rf /tmp/AirPortAtheros9271.kext
	@sudo cp -R /Users/pastel/Library/Developer/Xcode/DerivedData/AirPortAtheros9271-giyxvwyqrbunxsdenfzmxouaixip/Build/Products/Debug/AirPortAtheros9271.kext /tmp
	@sudo kextutil -n -t /tmp/AirPortAtheros9271.kext

.PHONY: check.info
check.info:
	@echo "Checking usb infos..."
	@ioreg -p IOUSB -l -w 0

.PHONY: logs
logs:
	@echo "Checking logs..."
	@log show --predicate 'senderImagePath CONTAINS "Atheros"' --style syslog --info --debug --source --last 5m

.PHONY: tail
tail:
	@echo "Streaming logs..."
	@log stream --predicate 'senderImagePath CONTAINS "Atheros"' --style syslog --level debug --source

.PHONY: load
load:
	@echo "Loading kext..."
	@sudo cp -R /Users/pastel/Library/Developer/Xcode/DerivedData/AirPortAtheros9271-giyxvwyqrbunxsdenfzmxouaixip/Build/Products/Debug/AirPortAtheros9271.kext /tmp
	@sudo kextutil -n -t /tmp/AirPortAtheros9271.kext

	@sudo kextutil -v 4 /tmp/AirPortAtheros9271.kext

.PHONY: unload
unload:
	@echo "Unloading kext..."
	@sudo kextunload /tmp/AirPortAtheros9271.kext
