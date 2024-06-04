package main

import (
	"encoding/json"
	"fmt"
	"google.golang.org/grpc"
	"github.com/simplePBFT/config"
	"github.com/simplePBFT/network"
	"github.com/simplePBFT/protos"
	"golang.org/x/net/context"
	"io/ioutil"
	"log"
	"net"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"strings"
)

const (
	defaultConfigPath = "config0.json"
)
type gserver struct{}
func (s *gserver) SayHello(ctx context.Context, in *protos.HelloRequest) (*protos.Block, error) {

	block,_:=network.MyMockLedger.GetBlock(uint64(in.Blockid))
	return block, nil
}
func main() {
	//加载配置文件
	path := defaultConfigPath
	if len(os.Args)>1 {
		path = os.Args[1]
	}
	config, err:= LoadConfig(path)
	if err != nil {
		panic(err)
	}

	//启动服务
	server := network.NewServer(config)
	server.Start()
	if strings.Compare(config.LocalNodeID,"cmt1_0")==0{
		lis, err := net.Listen("tcp", ":50051")
		if err != nil {
			log.Fatalf("failed to listen: %v", err)
		}
		gs := grpc.NewServer()
		protos.RegisterGreeterServer(gs, &gserver{})
		if err := gs.Serve(lis); err != nil {
			log.Fatalf("failed to serve: %v", err)
		}
	}

	//等待quit信号
	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Interrupt, os.Kill)
	s := <-c
	fmt.Println("Exit:", s)
	os.Exit(0)
}

func LoadConfig(path string) (*config.Config, error) {
	_, err := os.Stat(path)
	if err != nil {
		return nil, err
	}

	jsonBytes,err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}

	var config config.Config
	err = json.Unmarshal(jsonBytes, &config)
	if err != nil {
		return nil, err
	}
	return &config, nil
}
