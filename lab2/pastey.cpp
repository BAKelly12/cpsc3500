	/* Tell all the others to wait */
		for(auto &it: rq)
		{
			if(rq[it.first].activated && !rq[it.first].done && !rq[it.first].running)
			{
				rq[it.first].waitT++;
			}
		}
		




