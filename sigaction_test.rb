require './sigaction'

SigAction.set_signal_handler(Signal.list["TERM"])
SigAction.set_signal_handler(Signal.list["INT"])
SigAction.set_signal_handler(Signal.list["QUIT"])

loop do
  sleep 1
end
