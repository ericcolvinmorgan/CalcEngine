
import chalice.cli as ch
import os

# Potentially hacky debugging solution, but the docs were minimal in this area.
current_directory = os.getcwd()
fac = ch.factory.CLIFactory(current_directory + "/Web.API/api")
ch.run_local_server(fac, "127.0.0.1", 5678, "api")
