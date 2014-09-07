ALL	=	test_protobuf_press

all	:	$(ALL)

.cc.o	:
	g++ -I. -c $<

test_protobuf_press	:	test_protobuf_press.o	PressProtobuf.pb.cc
	g++ -o $@ $^ -L/usr/local/lib -lprotobuf -lpthread

clean	:
	rm -f *.o $(ALL)

