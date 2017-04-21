package modules.node;


moduleinterface Inode
{
    parameters:
	string RS;
	string FS;
	int dp;
	int rp;

	int level;
	double betweenness;

    gates:
	inout face[];	
	inout client_port;

}

