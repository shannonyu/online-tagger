INCLUDE_DIRECTORIES(
    ./;
    ./framework;
    ./core;
    ./math;
    ./utils)

ADD_EXECUTABLE(postag;
    ./demo/postag/main.cpp;
    ./demo/postag/learn.cpp;
    ./demo/postag/tag.cpp;
    ./core/c-parameter.cpp;
    ./core/hash-alphabet.cpp;
    ./core/c-data.cpp;
    ./core/cpp-data.cpp;
    ./core/ap-trainer.cpp;
    ./core/mira-trainer.cpp;
    ./core/pos-decoder.cpp;
    ./core/online-learner.cpp;
    ./core/online-tagger.cpp;
    ./core/model.cpp;
    ./corpus.cpp;
    ./pos-evaluator.cpp;
    ./pos-extractor.cpp;
    ./pos-reader.cpp;
    ./utils/cppstrlib.cpp;
    ./utils/ltp-argtable.cpp;
    ./utils/ltp-configure.cpp;
    ./utils/ltp-log.cpp)

ADD_EXECUTABLE(segment;
    ./demo/segment/main.cpp;
    ./demo/segment/learn.cpp;
    ./demo/segment/tag.cpp;
    ./core/c-parameter.cpp;
    ./core/hash-alphabet.cpp;
    ./core/c-data.cpp;
    ./core/cpp-data.cpp;
    ./core/ap-trainer.cpp;
    ./core/mira-trainer.cpp;
    ./core/seg-decoder.cpp;
    ./core/model.cpp;
    ./core/online-learner.cpp;
    ./core/online-tagger.cpp;
    ./corpus.cpp;
    ./seg-evaluator.cpp;
    ./seg-extractor.cpp;
    ./seg-reader.cpp;
    ./utils/cppstrlib.cpp;
    ./utils/ltp-argtable.cpp;
    ./utils/ltp-configure.cpp;
    ./utils/ltp-log.cpp)

#ADD_EXECUTABLE(dump_crfsuite;
#    ./tools/dump_crfsuite/main.cpp;
#    ./alphabet.cpp;
#    ./data.cpp;
#    ./corpus.cpp;
#    ./postag-reader.cpp;
#    ./postag-extractor.cpp;
#    ./segment-reader.cpp;
#    ./segment-extractor.cpp;
#    ./utils/cppstrlib.cpp;
#    ./utils/ltp-log.cpp;
#    ./utils/ltp-configure.cpp;
#    ./utils/ltp-argtable.cpp)
# 
# ADD_EXECUTABLE(onlinetagger;
#     ./demo/frontend/main.cpp;
#     ./demo/frontend/learn.cpp;
#     ./demo/frontend/tag.cpp;
#     ./alphabet.cpp;
#     ./data.cpp;
#     ./index-builder.cpp;
#     ./decoder.cpp;
#     ./model.cpp;
#     ./trainer.cpp;
#     ./parameter.cpp;
#     ./online-learner.cpp;
#     ./tagger-evaluator.cpp;
#     ./utils/ltp-argtable.cpp;
#     ./utils/ltp-configure.cpp;
#     ./utils/ltp-log.cpp;
#     ./utils/cppstrlib.cpp)
