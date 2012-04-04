-module(complex).

-export([foo/1, bar/1]).
-export([start/0, init/0]).

start() ->
	case erl_ddll:load_driver("./priv", "complex_drv") of
	ok -> 
		ok;
	{error, already_loaded} -> 
		ok;
	Error -> 
		io:format("~p~n", [Error]),
		exit(ferror, could_not_load_driver)
	end,
	spawn(?MODULE, init, []).

init() ->
	register(complex, self()),
	Port = open_port({spawn, "complex_drv"}, []),
	loop(Port).

foo(X) ->
	call_port({foo, X}).
bar(Y) ->
	call_port({bar, Y}).

call_port(Msg) ->
	complex ! {call, self(), Msg},
	receive 
	{complex, Result} ->
	Result
	end.

loop(Port) ->
	receive
	{call, Caller, Msg} ->
		Port ! {self(), {command, encode(Msg)}},
		receive
		{Port, {data, Data}} ->
			Caller ! {complex, decode(Data)}
		end,
		loop(Port)
	end.

encode({foo, X}) -> [1, X];
encode({bar, Y}) -> [2, Y].

decode([Int]) -> Int.


