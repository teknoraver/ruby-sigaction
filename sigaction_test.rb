require './sigaction'

def signal_handler()
  puts "Signal received"
end

SigAction.set_signal_handler(Signal.list["TERM"], 'signal_handler')
SigAction.set_signal_handler(Signal.list["INT"], 'signal_handler')
SigAction.set_signal_handler(Signal.list["QUIT"], 'signal_handler')

loop do
  sleep 1
  puts 'Sleeping'
end
