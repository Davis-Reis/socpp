# socpp
Socket library for cpp

# about
This was originally a part of another project but the scope became larger than the project itself. The purpose is to create a cpp wrapper for sockets that don't violate RAII.

# use
Uses a factory model to create sockets for different uses. Directly call socpp::Socket::connect()/listen()/accept() to create sockets for for those specific purposes.
