
if(CLANGINTROSPECTOR_EXECUTABLE)
	add_library(libwrappertest binary.cc)

	set(MAHHEADERS
		../../src/opa/script.h 
		../../src/opa/baseproxy.h 
		../../src/opa/converter.h
		../../src/opa/defs.h
		../../src/opa/exceptions.h)
		
	ouroboros_export_metadata(libwrappertest ${MAHHEADERS})
	ouroboros_create_wrappers(libwrappertest lua)
	ouroboros_create_wrappers(libwrappertest python)
	
endif()