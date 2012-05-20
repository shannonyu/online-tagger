/*
 *
 *
 *
 *
 *
 *
 *
 *
 */

#ifndef __PERCEPTRON_TRAINER_H__
#define __PERCEPTRON_TRAINER_H__

class PerceptronTrainer : public Trainer {
public:
    PerceptronTrainer(IndexBuilder *builder) : m_IdBuilder(builder) {}
    ~PerceptronTrainer() {}

    void train(Instance *inst, DecodeResults *result, Parameter *param, double curUpdSeq);
private:
    IndexBuilder *m_IdBuilder;
};

#endif  // end for __PERCEPTRON_TRAINER_H__
