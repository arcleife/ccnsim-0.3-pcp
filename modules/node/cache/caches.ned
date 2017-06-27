package modules.node.cache;

moduleinterface cache
{
    parameters:
	int C;
    gates:
	inout cache_port;
}

simple base_cache like cache
{
    parameters:
    	@display("i=block/buffer2;is=l");

	string DS = default("lce");
	int C = default (100);
	int theta1 = default (2);
	int theta2 = default (2);
    gates:
	inout cache_port;
}



simple random_cache extends base_cache{
    @class(random_cache);
}

simple lru_cache extends base_cache{
    @class(lru_cache);
}

simple two_cache extends base_cache{
    @class(two_cache);
}

simple fifo_cache extends base_cache{
    @class(fifo_cache);
}

simple pcp_i_cache extends base_cache{
	@class(pcp_i_cache);
}

simple pcp_e_cache extends base_cache{
	@class(pcp_e_cache);
}

simple lrud_cache extends base_cache{
	@class(lrud_cache);
}