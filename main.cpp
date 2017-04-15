#include <iostream>

#include "llvm/ADT/Optional.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

static llvm::LLVMContext s_Context;
static llvm::IRBuilder<> s_Builder(s_Context);
static std::unique_ptr<llvm::Module> s_Module;

int main()
{
	std::cout << "[asdf JIT compiler]\n";
	std::cout << "Note that log below is not actually happening here; it's status of code generating for those behaviors.\n";

	std::clog << "[LOG] Making module...\n";
	s_Module = llvm::make_unique<llvm::Module>("top", s_Context);
	
	std::clog << "[LOG] Defining main entry func...\n";
	llvm::Function* main_func = llvm::Function::Create
		(
			llvm::FunctionType::get(llvm::Type::getInt32Ty(s_Context), false),
			llvm::Function::ExternalLinkage, 
			"main",
			s_Module.get()
		);

	std::clog << "[LOG] Setting insert point...\n";
	s_Builder.SetInsertPoint(llvm::BasicBlock::Create(s_Context, "", main_func));

	constexpr std::size_t arr_size = 3000;

	std::clog << "[LOG] Declaring data and pointer...\n";
	llvm::Value* data = s_Builder.CreateAlloca(s_Builder.getInt8PtrTy(), nullptr, "data");
	llvm::Value* ptr = s_Builder.CreateAlloca(s_Builder.getInt8PtrTy(), nullptr, "ptr");

	std::clog << "[LOG] Getting standard calloc function...\n";
	llvm::Function* calloc_func = llvm::cast<llvm::Function>
		(
		 s_Module->getOrInsertFunction("calloc", s_Builder.getInt8PtrTy(), s_Builder.getInt64Ty(), s_Builder.getInt64Ty(), nullptr)
		);

	std::clog << "[LOG] Allocating data pointer...(with calloc)\n";
	llvm::Value* data_ptr = s_Builder.CreateCall(calloc_func, {s_Builder.getInt64(arr_size), s_Builder.getInt64(1)});

	std::clog << "[LOG] Storing to data and pointer...\n";
	s_Builder.CreateStore(data_ptr, data);
	s_Builder.CreateStore(data_ptr, ptr);

	llvm::Function* funcPutChar = llvm::cast<llvm::Function>(
			s_Module->getOrInsertFunction("putchar",
				s_Builder.getInt32Ty(),
				s_Builder.getInt32Ty(),
				nullptr));
	s_Builder.CreateCall(
			funcPutChar,
			{s_Builder.getInt32(65)}
			);

	std::clog << "[LOG] Getting standard free function...\n";
	llvm::Function* free_func = llvm::cast<llvm::Function>(
      	s_Module->getOrInsertFunction("free", s_Builder.getVoidTy(), s_Builder.getInt8PtrTy(),
        nullptr));

	std::clog << "[LOG] Free-ing allocated data...\n";
  	s_Builder.CreateCall(free_func, {s_Builder.CreateLoad(data)});

	std::clog << "[LOG] Returning zero from main entry function...\n";
  	s_Builder.CreateRet(s_Builder.getInt32(0));

/////////////////////////////////////////////////////////////////////////////
	/*llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

	std::string TargetTriple = llvm::sys::getDefaultTargetTriple();

	std::string err;
	const llvm::Target* Target = llvm::TargetRegistry::lookupTarget(TargetTriple, err);
	if (!Target) {
		std::cerr << "Failed to lookup target " + TargetTriple + ": " + err;
		return 1;
	}

	llvm::TargetOptions opt;
	auto RM = llvm::Optional<llvm::Reloc::Model>();
	llvm::TargetMachine* TheTargetMachine = Target->createTargetMachine(TargetTriple, "generic", "", opt, RM.getValueOr(llvm::Reloc::Model()));

	s_Module->setTargetTriple(TargetTriple);
	s_Module->setDataLayout(TheTargetMachine->createDataLayout());

	std::string Filename = "output.o";
	std::error_code err_code;
	llvm::raw_fd_ostream dest(Filename, err_code, llvm::sys::fs::F_None);
	if (err_code) {
		std::cerr << "Could not open file: " << err_code.message();
		return 1;
	}

	llvm::legacy::PassManager pass;
	if (TheTargetMachine->addPassesToEmitFile(pass, dest, llvm::TargetMachine::CGFT_ObjectFile)) {
		std::cerr << "TheTargetMachine can't emit a file of this type\n";
		return 1;
	}
	pass.run(*s_Module);
	dest.flush();
	std::cout << "Wrote " << Filename << "\n";*/

	std::error_code err_code;
	llvm::raw_fd_ostream output("output.ll", err_code, llvm::sys::fs::F_None);

	s_Module->print(output, nullptr);

	std::clog << "[LOG] Finished\n";
	return 0;
}
