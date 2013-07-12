This is my first watchface (and Github!).  This is a binary clock, and can be a little confusing to look at.

Binary looks likes 101001, and in base ten (normal numbers) that is 1*1+2*0+4*1+8*1+16*1+32*1 = 37.  

With the watch the basic idea is that there are three columns, one for the hour and two for the minutes.  The big squares on the left are the hour column, and the small squares to the right are the minute squares.



The arrangement of the watch is like this:


	8	8	8
	4	4	4	
	2	2	2
	1	1	1
      Hours    Ten    Single
	     Minutes  Minutes 


For example:

 ___
|   |
|___| 




 ___
|   |  __
|___| |__|

	    __
	   |__|
Hours  ten  single
       min   min


Would be 8 + 2 hours = 10
2 ten minutes = 2
1 single minute
So the time would be 10 hours, and 20+1=21 minutes, or 10:21.

Currently the watch only supports 12 hour time, and will not change if 24 hour time is selected.

The time in the picture is 10:25!



