package memoryproducer

type Broker interface {
	send([]byte) error
}

type Producer struct {
	broker       Broker
	buffer       []byte
	maxBatchSize int
}

func NewProducer(broker Broker, maxBatchSize int) *Producer {
	return &Producer{
		broker:       broker,
		maxBatchSize: maxBatchSize,
	}
}

func (p *Producer) Send(buffer []byte) error {
	if len(p.buffer)+len(buffer) > p.maxBatchSize {
		return p.flush()
	}
	p.buffer = append(p.buffer, buffer...)

	return nil
}

func (p *Producer) flush() error {
	if len(p.buffer) == 0 {
		return nil
	}

	if err := p.broker.send(p.buffer); err != nil {
		return err
	}

	p.buffer = nil

	return nil
}
