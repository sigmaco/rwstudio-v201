
NetTestps.dll: dlldata.obj NetTest_p.obj NetTest_i.obj
	link /dll /out:NetTestps.dll /def:NetTestps.def /entry:DllMain dlldata.obj NetTest_p.obj NetTest_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0500 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del NetTestps.dll
	@del NetTestps.lib
	@del NetTestps.exp
	@del dlldata.obj
	@del NetTest_p.obj
	@del NetTest_i.obj
